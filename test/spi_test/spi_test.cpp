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

#include "../../src/MCP3004Comm.h"

class MCP3004printSampleCallback : public MCP3004callback {
        virtual void hasSample(int v, int c) {
                printf("CH%d, value = %d\n",c,v);
        }
};


int main(int argc, char *argv[]) {
        MCP3004Comm* m = new MCP3004Comm();
    MCP3004printSampleCallback print_cb;
    m->setCallback(&print_cb);
    m->start();

    getchar();
    m->stop();
    delete m;

    return 0;
}

