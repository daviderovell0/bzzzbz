#include "MCP3008Comm.h"


MCP3008Comm::MCP3008Comm(const char* spiDevice) {
	fd = open(spiDevice, O_RDWR);
	if (fd < 0)
		throw "can't open device";
	
	// set SPI mode
	int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1) {
		throw "can't set spi mode";
	} else {
		fprintf(stderr,"SPI mode %d set (ret=%d).\n",mode,ret);
	}	
}

void MCP3008Comm::setCallback(MCP3008callback* cb) {
	mcp3008callback = cb;
}

int MCP3008Comm::spi_transfer(int fd, uint8_t* tx, uint8_t* rx, int n) {
	struct spi_ioc_transfer tr;
	memset(&tr,0,sizeof(struct spi_ioc_transfer));
	tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)rx;
	tr.len = n;
	tr.delay_usecs = delay;
	tr.speed_hz = speed;
	tr.bits_per_word = bpw;	
	int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	return ret;
}


int MCP3008Comm::readData(int fd, uint8_t ch) {
	uint8_t tx[3] = {1,ch,0};
	uint8_t rx[3] = {0,0,0};
	
	gpio_set_value(CS,0);

	int ret = spi_transfer(fd, tx, rx, 3);
	if (ret < 1) {
		printf("\n can't send spi message, ret = %d\n",ret);
		throw "Can't send spi message";
	}
	
	gpio_set_value(CS,0);

	return ((rx[1]<<8)|(rx[2])& MASK); // get 10bits LSB
}


void MCP3008Comm::run(MCP3008Comm* mcp3008comm) {
	int channel = 3;
	mcp3008comm->running = 1;
	while (mcp3008comm->running) {
		
		for (channel; channel< 6; channel++) {
			// read the data register by performing two 8 bit reads
			int value = mcp3008comm->readData(mcp3008comm->fd, mcp3008comm->SINGLE_CHANNEL[channel]);
			if (mcp3008comm->mcp3008callback) {
				mcp3008comm->mcp3008callback->hasSample(value, channel);
			}
		}
	channel = 3;
	}
	close(mcp3008comm->fd);
}


void MCP3008Comm::start() {
	if (daqThread) {
		throw "Called while DAQ is already running.";
	}
	
	// Pull CS up
	gpio_export(CS);
	gpio_set_dir(CS,1); // set to out
	gpio_set_value(CS,1);

	fprintf(stderr,"Receiving data.\n");
	
	daqThread = new std::thread(run,this);
}


void MCP3008Comm::stop() {
	running = 0;
	gpio_unexport(CS);

	if (daqThread) {
		daqThread->join();
		delete daqThread;
		daqThread = NULL;
	}
}
