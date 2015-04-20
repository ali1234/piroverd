/* i2c.c -- i2c helper functions
 *
 * Copyright (C) 2015 Alistair Buxton <a.j.buxton@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/i2c-dev.h>

#include "i2c.h"

int i2c_open(int adapter, int address)
{
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", adapter);
    int device = open(filename, O_RDWR|O_NONBLOCK);
    if (device < 0) {
        fprintf(stderr, "Error opening i2c adapter.\n");
        return -1;
    }
    if (ioctl(device, I2C_SLAVE, address) < 0) {
        fprintf(stderr, "Error setting i2c device address.\n");
        return -1;
    }
    return device;
}

void i2c_close(int device)
{
    close(device);
}
