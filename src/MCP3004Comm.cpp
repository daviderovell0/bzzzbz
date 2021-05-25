#include "MCP3004Comm.h"


MCP3004Comm::MCP3004Comm(const char* spiDevice) {
	fd = open(spiDevice, O_RDWR);
	if (fd < 0)
		throw "can't open SPI device";
	
	// set SPI mode
	int ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1) {
		throw "can't set SPI mode";
	} else {
		fprintf(stderr,"SPI mode %d set (ret=%d).\n",mode,ret);
	}	
}

void MCP3004Comm::setCallback(MCP3004callback* cb) {
	mcp3004callback = cb;
}

int MCP3004Comm::spi_transfer(int fd, uint8_t* tx, uint8_t* rx, int n) {
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


int MCP3004Comm::readData(int fd, uint8_t ch) {
	uint8_t tx[3] = {1,ch,0};
	uint8_t rx[3] = {0,0,0};
	
	//gpio_set_value(CS,0); //throwing error

	int ret = spi_transfer(fd, tx, rx, 3);
	if (ret < 1) {
		printf("\n can't send spi message, ret = %d\n",ret);
		throw "Can't send spi message";
	}
	
	//gpio_set_value(CS,0); //throwing error?

	return ((rx[1]<<8)|(rx[2])& MASK); // get 10bits LSB
}


void MCP3004Comm::run(MCP3004Comm* mcp3004comm) {
	int channel = 0;
	mcp3004comm->running = 1;
	while (mcp3004comm->running) {
		
		for (channel; channel< 4; channel++) {
			// read the data register by performing two 8 bit reads
			int value = mcp3004comm->readData(mcp3004comm->fd, mcp3004comm->SINGLE_CHANNEL[channel]);
			if (mcp3004comm->mcp3004callback) {
				mcp3004comm->mcp3004callback->hasSample(value, channel);
			}
		}
	channel = 0;
	}
	close(mcp3004comm->fd);
}


void MCP3004Comm::start() {
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


void MCP3004Comm::stop() {
	running = 0;
	gpio_unexport(CS);

	if (daqThread) {
		daqThread->join();
		delete daqThread;
		daqThread = NULL;
	}
}
