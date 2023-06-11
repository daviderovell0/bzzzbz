#include "NHD223Comm.h"



int main() {

    NHD223Comm* display = new NHD223Comm("/dev/spidev5.0"); //SPI5

    display->close();

}
