CC	= gcc
CFLAGS	= -O2 -Wall -lm

pi: simple_jack_client.c
		${CC} ${CFLAGS} `pkg-config --cflags --libs jack` -o audioin.o simple_jack_client.c

ubuntu: simple_jack_client.c
		${CC} -o audioin.o simple_jack_client.c /usr/lib/x86_64-linux-gnu/libjack.so ${CFLAGS}