/*
 * @Copyright (c) 2020, MADMACHINE LIMITED
 * @Author: Frank Li(lgl88911@163.com)
 * @SPDX-License-Identifier: MIT
 */

struct mad_mp3dec_pcm_info {
	int channels;
	int sample_bits;
	int sample_rate;
};

typedef struct mad_mp3dec_pcm_info mad_mp3dec_pcm_info_t;

int mad_mp3dec_init(void);
int mad_mp3dec_feeddata(char *buf, int len);
int mad_mp3dec_decode(char *buf, int *len, mad_mp3dec_pcm_info_t *pcm_info, char *tail, int *tail_len);
int mad_mp3dec_deinit(void);





