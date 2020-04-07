ubuntu_test: AudioProcessing.cpp AudioProcessing.h
	g++ -Wall -lm -o printaudio.o main.cpp AudioProcessing.cpp /usr/lib/x86_64-linux-gnu/libjack.so -lkiss-fft
pi: AudioProcessing.cpp AudioProcessing.h
	g++ -Wall -lm -ljack -lGL -lglut  -lGLEW main.cpp shader_utils.cpp AudioProcessing.cpp -o bz.o -lkiss-fft