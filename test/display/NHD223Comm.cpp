#include "NHD223Comm.h"
#include "GPIODUtils.h"
#include <sys/ioctl.h>
#include <iostream>

#include <unistd.h>


NHD223Comm::NHD223Comm(const char* spiDevice) {
	fd = open(spiDevice, O_RDWR);
	if (fd < 0)
		throw "can't open SPI device";
	
	// set SPI mode (WR)
	int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1) {
		throw "can't set SPI mode";
	} else {
		fprintf(stderr,"SPI mode %d set (ret=%d).\n",mode,ret);
	}

	// setting the spi_ioc_transfer struct
	memset(&tr,0,sizeof(struct spi_ioc_transfer));

	tr.delay_usecs = delay;
	tr.speed_hz = speed;
	tr.bits_per_word = bpw;	

	// init GPIO utils chip

	//GPIODUtils* display = new GPIODUtils();


}

int NHD223Comm::on() {
	return 1;
}

int NHD223Comm::spi_transfer(uint8_t* tx, uint8_t* rx, int n) {
	tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)rx;
	tr.len = n;
	int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	return ret;
}

void NHD223Comm::close() {
	// close SPI device
	::close(fd);
}