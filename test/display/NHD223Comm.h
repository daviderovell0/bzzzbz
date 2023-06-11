/** @file NHD223Comm
 * 
 * @brief Class to communicate with the NHD-2.23-128-32 display on bzzzbz board v2.0 via SPI5 on the pi4
 * @author daviderovell0
 */

#ifndef __NHD223COMM_H
#define __NHD223COMM_H

#include <stdint.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

class NHD223Comm {
    public:
        /**
         * \brief Constructor with the spiDevice. The default device
         * is /dev/spidev3.0 = SPI5.
         * 
         * \param const char* spiDevice - the SPI interface of the Raspberry Pi.
         **/
	     NHD223Comm(const char* spiDevice = "/dev/spidev5.0");
         int spi_transfer(uint8_t* tx, uint8_t* rx, int n);
         int on();
         int off();

         void close();
         

    private:

        int fd = 0;
        struct spi_ioc_transfer tr;
        const uint8_t mode = SPI_MODE_0;
	    const uint32_t speed = 500000;
	    const uint16_t delay = 0;
	    const uint8_t bpw   = 8;
};

#endif
