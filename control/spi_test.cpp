/*
 * AD7705 test/demo program for the Raspberry PI
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 * Copyright (c) 2013-2020  Bernd Porr <mail@berndporr.me.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "MCP3008Comm.h"

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