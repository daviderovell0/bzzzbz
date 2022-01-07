// #include "NHD223Comm.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "../../src/gpio-sysfs.h"

int spi_transfer(int fd, uint8_t* tx, uint8_t* rx, int n) {
	struct spi_ioc_transfer tr;
	memset(&tr,0,sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)rx;
	tr.len = n;
	tr.delay_usecs = 0;
	tr.speed_hz = 500000;
	tr.bits_per_word = 8;	
	int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	return ret;
}

int main(int argc, char *argv[])
{
	// NHD223Comm* display = new NHD223Comm();

    // set CS (GPIO12) low
	// gpio_export(12);
	// gpio_set_dir(12,1); // set to out
	// gpio_set_value(12,0);

    // set D/C (GPIO16) to low -> command mode
	gpio_export(16);
	gpio_set_dir(16,1); // set to out
	gpio_set_value(16,0);

    // hardware reset screen
    gpio_export(17);
	gpio_set_dir(17,1); // set to out
	gpio_set_value(17,0); 

	gpio_set_value(17,1);

    // set spi_transfer args
    int fd = 0;
	const uint8_t mode = SPI_MODE_0;
	const char *device = "/dev/spidev3.0";
    
	fd = open(device, O_RDWR);
	if (fd < 0)
		throw "can't open SPI device";
	
	// set SPI mode (WR)
	int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1) {
		throw "can't set SPI mode";
	} else {
		fprintf(stderr,"SPI mode %d set (ret=%d).\n",mode,ret);
	}


	// perform the transfers
	uint8_t tx[1] = {0xAF}; // Set display on
	uint8_t rx[1];

    ret = spi_transfer(fd, tx, rx, 1);
	if (ret < 1) {
		printf("\n can't send spi message, ret = %d\n",ret);
		throw "Can't send spi message";
	}


    tx[0] = 0xA5; // Entire display on

    ret = spi_transfer(fd, tx, rx, 1);
	if (ret < 1) {
		printf("\n can't send spi message, ret = %d\n",ret);
		throw "Can't send spi message";
	}

    
}