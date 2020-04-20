/** @file spi_test.cpp
 * 
 * @brief Test file to test SPI data readout. Tweak the knobs to check if the 
 * values change accordingly  
 *
 * @author Davide Rovelli (daviderovell0)
 * 
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "../MCP3008Comm.h"

class MCP3008printSampleCallback : public MCP3008callback {
	virtual void hasSample(int v, int c) {
		printf("CH%d, value = %d\n",c,v);
	}
};


int main(int argc, char *argv[]) {
	MCP3008Comm* m = new MCP3008Comm();
    MCP3008printSampleCallback print_cb;
    m->setCallback(&print_cb);
    m->start();

    getchar();
    m->stop();
    delete m;

	return 0;
}
