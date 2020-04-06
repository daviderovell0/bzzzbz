output: AudioProcessing.cpp AudioProcessing.h
	g++ -Wall -lm -o printaudio.o main.cpp AudioProcessing.cpp /usr/lib/x86_64-linux-gnu/libjack.so -lkiss-fft
pi: AudioProcessing.cpp AudioProcessing.h
	g++ -Wall -lm -o -ljack printaudio.o main.cpp AudioProcessing.cpp -lkiss-fft