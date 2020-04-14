Real Time Embedded Programming 5 repository. Team 9 - Davide Rovelli, Marcell Illyes, Peter Nagy. 
# bzzzbz - Digital Video Synthesizer
**bzzzbz** is a video synthesizer that dynamically reacts to an audio input to create images and visuals that can be manipulated for integrated audio-visual performances.  <br>
This product is meant to be a simpler and cheaper alternative to commercial analog video synthesizers, still providing a flexible interface to enhance musical expression.
## Specifications
The device will use a Raspberry Pi to process incoming audio and elaborate real-time graphical patterns. The Raspberry Pi will be also connected to a control surface that allows the user to modify the graphics dynamically.
## Installation
On Raspberry Pi 3B+: 
- Install OpenGL libraries: freeglut3, freeglut3-dev, libgl1-mesa-dev, mesa-common-dev, libglew-dev
- Turn on FKMS in raspi-config to enable GPU acceleration
- install raspberrypi-kernel-headers
- Compile kissFFT
- cmake, make install
### Source code
*Device under development*
