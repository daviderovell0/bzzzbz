CC	= gcc
CFLAGS	= -O2 -Wall `pkg-config --cflags --libs jack` -lm

audioin: simple_jack_client.c
		${CC} ${CFLAGS} -o audioin.o simple_jack_client.c

