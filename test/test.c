/*
 * @Copyright (c) 2020, MADMACHINE LIMITED
 * @Author: Frank Li(lgl88911@163.com)
 * @SPDX-License-Identifier: MIT
 */

#include "../MadMP3Dec.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define READBUFSZ (21060)
static char readBuf[READBUFSZ];

#define DECBUFSZ (210600)
static char decBuf[DECBUFSZ];


int main(int argc, char *argv[])
{
	char *input;
	char *output;
	FILE *ifile, *ofile;
	int readlen, readed;
	int tail;

	if (argc != 3) {
		printf("param inval\n");
		return 1;
	}

	input = argv[1];
	output = argv[2];

	printf("dec %s to %s\n", input, output);

	ifile = fopen(input, "rb");
	ofile = fopen(output, "wb");

	if (ifile == NULL || ofile == NULL) {
		printf("open file fail %d %s\n", errno, strerror(errno));
		return 1;
	}

	madMP3Dec_deinit();
	tail = 0;
	while (1) {
		readlen = READBUFSZ - tail;
		readed = fread(readBuf + tail, 1, readlen, ifile);
		if (readed > 0) {
			printf("Feed data %d\n", readed + tail);
			madMP3Dec_feeddata(readBuf, readed + tail);
		}

		while (1) {
			int bufferlen = DECBUFSZ;
			int declen;
			MP3Dec_PCMInfo_t pcminfo;
			tail = 0;
			declen = madMP3Dec_decode(decBuf, &bufferlen, &pcminfo, readBuf, &tail);
			printf("Decode pcm data %d\n", declen);

			if (declen > 0) {
				printf("Write pcm %d[chan %d, bit %d, freq %d] tail %d\n",
				       declen, pcminfo.channels, pcminfo.bit, pcminfo.samplerate, tail);
				fwrite(decBuf, 1, declen, ofile);
			} else  {
				printf("No data check pcm info[chan %d, bit %d, freq %d] tail %d\n",
				       pcminfo.channels, pcminfo.bit, pcminfo.samplerate, tail);
			}

			if (bufferlen == 0) {
				printf("Cur decoder end\n\n");
				break;
			}
		}

		if (readed < readlen) {
			printf("All decoder finish\n");
			break;
		}
	}

	madMP3Dec_deinit();

	fclose(ifile);
	fclose(ofile);

	return 0;
}

