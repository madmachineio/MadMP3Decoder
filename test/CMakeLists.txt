PROJECT(Demo)

SET(CMAKE_C_COMPILER gcc)

INCLUDE_DIRECTORIES(../)
INCLUDE_DIRECTORIES(../chal_export/include/)
INCLUDE_DIRECTORIES(../libmad-0.15.1b/)

ADD_COMPILE_OPTIONS(${C_FLAGS})
ADD_COMPILE_OPTIONS(-DHAVE_CONFIG_H)

SET(LIBMAD_SRC ../libmad-0.15.1b/bit.c
				../libmad-0.15.1b/decoder.c
				../libmad-0.15.1b/fixed.c
				../libmad-0.15.1b/frame.c
				../libmad-0.15.1b/huffman.c
				../libmad-0.15.1b/layer12.c
				../libmad-0.15.1b/layer3.c
				../libmad-0.15.1b/stream.c
				../libmad-0.15.1b/synth.c
				../libmad-0.15.1b/timer.c
				../libmad-0.15.1b/version.c
				../MadMP3Dec.c)
				
				
ADD_EXECUTABLE(dectest test.c ${LIBMAD_SRC})
