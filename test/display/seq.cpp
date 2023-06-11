#include "NHD223Comm.h"



int main() {

    NHD223Comm* display = new NHD223Comm("/dev/spidev3.0"); //SPI5

    display->close();

}
