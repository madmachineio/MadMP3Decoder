PROJECT(MADMP3DECODER)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../externlib/lib) 

include(./chal_export/flags.cmake)

SET(CMAKE_C_COMPILER arm-zephyr-eabi-gcc)
#SET(CMAKE_C_COMPILER gcc)

INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(./chal_export/include/)
INCLUDE_DIRECTORIES(./libmad-0.15.1b/)

ADD_COMPILE_OPTIONS(${C_FLAGS})
ADD_COMPILE_OPTIONS(-DHAVE_CONFIG_H)

SET(LIBMADMP3DECODER_SRC ./libmad-0.15.1b/bit.c
				./libmad-0.15.1b/decoder.c
				./libmad-0.15.1b/fixed.c
				./libmad-0.15.1b/frame.c
				./libmad-0.15.1b/huffman.c
				./libmad-0.15.1b/layer12.c
				./libmad-0.15.1b/layer3.c
				./libmad-0.15.1b/stream.c
				./libmad-0.15.1b/synth.c
				./libmad-0.15.1b/timer.c
				./libmad-0.15.1b/version.c
				./MadMP3Dec.c)
				
ADD_LIBRARY(madmp3decoder STATIC ${LIBMADMP3DECODER_SRC})

add_custom_command(TARGET madmp3decoder POST_BUILD
	COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/../externlib/inc
    COMMAND cp ${PROJECT_SOURCE_DIR}/MadMP3Dec.h ${PROJECT_SOURCE_DIR}/../externlib/inc)

#gcc ../libmad-0.15.1b/minimad.c -I../libmad-0.15.1b/ -L../../externlib/lib/ -lmadmp3decoder -o minimad
#./minimad < ~/test.mp3 1>test.pcm