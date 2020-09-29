/*
 * @Copyright (c) 2020, MADMACHINE LIMITED
 * @Author: Frank Li(lgl88911@163.com)
 * @SPDX-License-Identifier: MIT
 */

#include "mad.h"
#include "MadMP3Dec.h"

#define DBG             // printf
#define ABORT() DBG("abort %s %d", __FILE__, __LINE__); while (1);

#define OUTPUT_BIT_PER_SAMPLE   16
#define BIT_PER_BYTE    8
#define REMAINDER_SIZE  32 * 1024

#define NULL    ((void *)0)

struct mad_stream dec_stream;
struct mad_frame dec_frame;
struct mad_synth dec_synth;

static char rembuf[REMAINDER_SIZE];
static int remainder;
static char *rem_p;

static inline
signed int scale(mad_fixed_t sample)
{
	/* round */
	sample += (1L << (MAD_F_FRACBITS - 16));

	/* clip */
	if (sample >= MAD_F_ONE) {
		sample = MAD_F_ONE - 1;
	} else if (sample < -MAD_F_ONE) {
		sample = -MAD_F_ONE;
	}

	/* quantize */
	return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static
enum mad_flow error(void *data,
		    struct mad_stream *stream,
		    struct mad_frame *frame)
{

	DBG("decoding error 0x%04x (%s) at byte offset %u\n",
	    stream->error, mad_stream_errorstr(stream),
	    stream->this_frame);

	/* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

	return MAD_FLOW_CONTINUE;
}


int madMP3Dec_init(void)
{
	struct mad_stream *stream = &dec_stream;
	struct mad_frame *frame = &dec_frame;
	struct mad_synth *synth = &dec_synth;

	remainder = 0;
	rem_p = rembuf;

	mad_stream_init(stream);
	mad_frame_init(frame);
	mad_synth_init(synth);

	mad_stream_options(stream, 0);
}

int madMP3Dec_feeddata(char *buf, int len)
{
	struct mad_stream *stream = &dec_stream;

	mad_stream_buffer(stream, buf, len);

	return len;
}

int madMP3Dec_decode(char *buf, int *len, MP3Dec_PCMInfo_t *pcminfo, char *tail, int *taillen)
{
	struct mad_stream *stream = &dec_stream;
	struct mad_frame *frame = &dec_frame;
	struct mad_synth *synth = &dec_synth;
	mad_fixed_t const *left_ch, *right_ch;

	int buf_len = *len;
	int writed = 0;
	int pcm_samples = 0;

	// First Get pcm data from remainder
	if (remainder > 0) {
		DBG("remainder %d\n", remainder);
		if (remainder > buf_len) {
			memcpy(buf, rem_p, buf_len);
			rem_p += buf_len;
			remainder -= buf_len;
			*len = remainder;
			*taillen = stream->bufend - stream->next_frame;
			if (*taillen > 0) {
				memmove(tail, stream->next_frame, *taillen);
			}
			return buf_len;
		} else  {
			memcpy(buf, rem_p, remainder);
			writed = remainder;
			rem_p = rembuf;
			remainder = 0;
		}
	}

	while (1) {
		// decode
		if (mad_frame_decode(frame, stream) == -1) {
			if (!MAD_RECOVERABLE(stream->error)) {
				break;
			}

			error(NULL, stream, frame);
			continue;
		}

		// read decode data
		mad_synth_frame(synth, frame);

		pcminfo->bit = OUTPUT_BIT_PER_SAMPLE;
		pcminfo->channels = synth->pcm.channels;
		pcminfo->samplerate = synth->pcm.samplerate;
		pcm_samples = synth->pcm.length;

		left_ch = synth->pcm.samples[0];
		right_ch = synth->pcm.samples[1];

		while (pcm_samples--) {
			signed int sample;

			if (writed >= buf_len) {
				// output buffer full, keep pcm data to remainder buffer
				if (remainder >= REMAINDER_SIZE) {
					// ABORT, remainder buffer full
					ABORT();
				}
				sample = scale(*left_ch++);
				rembuf[writed] = (sample >> 0) & 0xff;
				remainder++;
				rembuf[writed] = (sample >> 8) & 0xff;
				remainder++;

				if (pcminfo->channels == 2) {
					sample = scale(*right_ch++);
					rembuf[writed] = (sample >> 0) & 0xff;
					remainder++;
					rembuf[writed] = (sample >> 8) & 0xff;
					remainder++;
				}
			} else  {
				// output pcm data
				sample = scale(*left_ch++);
				buf[writed] = (sample >> 0) & 0xff;
				writed++;
				buf[writed] = (sample >> 8) & 0xff;
				writed++;

				if (pcminfo->channels == 2) {
					sample = scale(*right_ch++);
					buf[writed] = (sample >> 0) & 0xff;
					writed++;
					buf[writed] = (sample >> 8) & 0xff;
					writed++;
				}
			}
		}
	}

	*len = remainder;
	// output haven't decode data
	*taillen = stream->bufend - stream->next_frame;
	if (*taillen > 0) {
		memmove(tail, stream->next_frame, *taillen);
	}
	return writed;
}

int madMP3Dec_deinit(void)
{
	struct mad_stream *stream = &dec_stream;
	struct mad_frame *frame = &dec_frame;
	struct mad_synth *synth = &dec_synth;

	mad_synth_finish(synth);
	mad_frame_finish(frame);
	mad_stream_finish(stream);
}

