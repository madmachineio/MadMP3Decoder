/*
 * @Copyright (c) 2020, MADMACHINE LIMITED
 * @Author: Frank Li(lgl88911@163.com)
 * @SPDX-License-Identifier: MIT
 */

typedef struct {
	int channels;
	int bit;
	int samplerate;
} MP3Dec_PCMInfo_t;

int madMP3Dec_init(void);
int madMP3Dec_feeddata(char *buf, int len);
int madMP3Dec_decode(char *buf, int *len, MP3Dec_PCMInfo_t *pcminfo, char *tail, int *taillen);
int madMP3Dec_deinit(void);





