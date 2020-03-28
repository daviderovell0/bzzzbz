CC	= gcc
CFLAGS	= -O2 -Wall -lm

pi: simple_jack_client_pi audioin_pi
ubuntu: simple_jack_client_ubuntu audioin_ubuntu


simple_jack_client_pi: simple_jack_client.c
		${CC} ${CFLAGS} `pkg-config --cflags --libs jack` -o sc.o simple_jack_client.c

simple_jack_client_ubuntu: simple_jack_client.c
		${CC} -o sc.o simple_jack_client.c /usr/lib/x86_64-linux-gnu/libjack.so ${CFLAGS}

audioin_pi: audioin.c
		${CC} ${CFLAGS} `pkg-config --cflags --libs jack` -o audioin.o audioin.c

audioin_ubuntu: audioin.c
		${CC} -o audioin.o audioin.c /usr/lib/x86_64-linux-gnu/libjack.so ${CFLAGS}