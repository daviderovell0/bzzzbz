## SPI acquisition test

This folder contains a simple program that prints the samples coming from the control knobs
of **bzzzbz**.

It can be used to understand how the SPI clients manages acquisition and processing works and also to 
test if the device receives SPI data. Run:
```
cmake .
make
./spi_test.o
```
If you see the samples varying accordingly to the input the audio is working.