output: AudioProcessing.cpp AudioProcessing.h
	g++ -Wall -lm -o printaudio.o main.cpp AudioProcessing.cpp /usr/lib/x86_64-linux-gnu/libjack.so