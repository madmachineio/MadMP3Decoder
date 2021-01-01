/*
 * @Copyright (c) 2020, MADMACHINE LIMITED
 * @Author: Frank Li(lgl88911@163.com)
 * @SPDX-License-Identifier: MIT
 */

/**
 * @brief MP3 decoder PCM info
 *
 * @param channels decoder data channel number
 * @param sample_bits decoder frame data sample bits
 * @param sample_rate decoder data sample rate
 */
struct mad_mp3dec_pcm_info {
	int channels;
	int sample_bits;
	int sample_rate;
};

typedef struct mad_mp3dec_pcm_info mad_mp3dec_pcm_info_t;

/**
 * @brief Initialize the MP3 decoder
 *
 * @retval 0 If successful.
 * @retval Negative errno code if failure.
 */
int mad_mp3dec_init(void);

/**
 * @brief Feed MP3 data to decode
 *
 * @param buf buffer of MP3 data
 * @param len feed size in byte
 *
 * @retval 0 If successful.
 * @retval Negative errno code if failure.
 */
int mad_mp3dec_feeddata(char *buf, int len);

/**
 * @brief Decode MP3 data to output pcm data and pcm data info
 *
 * The remaining undecoded data will be feed first next call mad_mp3dec_feeddata
 *
 * @param buf Buffer of output pcm data
 * @param len Output data size of pcm in byte
 * @param pcm_info Output pcm data info
 * @param tail Output the remaining data without decoding
 * @param tail_len Un-decoding data size in byte
 *
 * @retval 0 If successful.
 * @retval Negative errno code if failure.
 */
int mad_mp3dec_decode(char *buf, int *len, mad_mp3dec_pcm_info_t *pcm_info, char *tail, int *tail_len);

/**
 * @brief De-Init MP3 decoder
 *
 * @retval 0 If successful.
 * @retval Negative errno code if failure.
 */
int mad_mp3dec_deinit(void);





