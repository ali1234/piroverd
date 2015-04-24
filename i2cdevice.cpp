/* i2cdevice.cpp -- i2c device base class
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

#include "i2cdevice.h"

I2CDevice::I2CDevice(int adapter, int address)
{
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", adapter);
    file_des = open(filename, O_RDWR|O_NONBLOCK);
    if (file_des < 0) {
        fprintf(stderr, "Error opening i2c adapter.\n");
        throw file_des;
    }
    if (ioctl(file_des, I2C_SLAVE, address) < 0) {
        fprintf(stderr, "Error setting i2c device address.\n");
        throw file_des;
    }
}

I2CDevice::~I2CDevice()
{
    close(file_des);
}



__u8 I2CDevice::read8(__u8 reg)
{
    __s32 result = i2c_smbus_read_byte_data(file_des, reg);
//    if(result < 0)
//        throw result;
//    else
        return result;
}

void I2CDevice::write8(__u8 reg, __u8 val)
{
    __s32 result = i2c_smbus_write_byte_data(file_des, reg, val);
//    if(result < 0)
//        throw result;
}

__u16 I2CDevice::read16(__u8 reg)
{
    __s32 result = i2c_smbus_read_word_data(file_des, reg);
//    if(result < 0)
//        throw result;
//    else
        return result;
}

void I2CDevice::write16(__u8 reg, __u16 val)
{
    __s32 result = i2c_smbus_write_word_data(file_des, reg, val);
//    if(result < 0)
//        throw result;
}

