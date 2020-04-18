/** @file MCP3008Comm.h
 * 
 * @brief TO WRITE 
 *
 * @author BZZZBZ
 * 
 */

#ifndef __MCP3008COMM_H
#define __MCP3008COMM_H

/*
 * AD7705 class to read data at 50Hz sampling rate
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 * Copyright (c) 2013-2020  Bernd Porr <mail@berndporr.me.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
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
#include <thread>  

#include "gpio-sysfs.h"

/**
 * Callback for new samples which needs to be implemented by the main program.
 * The function hasSample needs to be overloaded in the main program.
 **/
class MCP3008callback {
public:
	/**
	 * Called after a sample has arrived.
	 **/
	virtual void hasSample(int value, int channel) = 0;
};


/**
 * This class reads data from the AD7705 in the background (separate
 * thread) and calls a callback function whenever data is available.
 **/
class MCP3008Comm {

public:
	/**
	 * Constructor with the spiDevice. The default device
	 * is /dev/spidev0.0.
	 **/
	MCP3008Comm(const char* spiDevice = "/dev/spidev0.0");

	/**
	 * Sets the callback which is called whenever there is a sample
	 **/
	void setCallback(MCP3008callback* cb);

	/**
	 * Starts the data acquisition in the background and the
	 * callback is called with new samples
	 **/
	void start();

	/**
	 * Stops the data acquistion
	 **/
	void stop();




private:
	const uint8_t SINGLE_CHANNEL [8] = {0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0};
	const uint8_t DIFF_CHANNEL [8] = {0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70};
	const uint16_t MASK = 1023; /* 10bit mask to filter useful data */
	const uint8_t CS = 8;
	const uint8_t mode = SPI_CPHA | SPI_CPOL;
	const uint32_t speed = 500000;
	const uint16_t delay = 0;
	const uint8_t bpw   = 8;
	
	int fd = 0;
	std::thread* daqThread = NULL;
	int running = 0;
	MCP3008callback* mcp3008callback = NULL;

	int spi_transfer(int fd, uint8_t* tx, uint8_t* rx, int n);
	int readData(int fd, uint8_t ch);
	static void run(MCP3008Comm* mcp3008comm);

};


#endif
