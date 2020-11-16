## Audio acquisition test

This folder contains a simple program that prints the samples coming from an audio port
of your device:
- For bzzzbz this would be the lineIN left channel of the stereo jack.
- For a laptop this is likely to be your build-in microphone.

It can be used to understand how the audio acquisition and processing works and also to 
test if the device receives audio data. Run:
```
cmake .
make
./audioin.o
```
If you see the samples varying accordingly to the input the audio is working.

### Plotting audio data
The *plot* folder contains a jupyter notebook that can be used to visualise audio data in the 
*samples* folder. The samples were recorded from **bzzzbz** line input. Files starting with *fft*
represent the frequency equivalent of the raw samples.
