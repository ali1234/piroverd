/* ak8975.c -- i2c magnetometer driver
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

#include <linux/i2c-dev.h>

#include "i2c.h"
#include "ak8975.h"

int min[3] = { -254, -97, -159 };
int max[3] = { 22, 191, 130 };

int ak8975_open(int adapter, int address)
{
    int device = i2c_open(adapter, address);

    if (device == -1)
        return -1;

    i2c_smbus_write_byte_data(device, 0x0A, 0x01);

    return device;
}

void ak8975_close(int device)
{
    i2c_smbus_write_byte_data(device, 0x0A, 0x00);

    i2c_close(device);
}

void ak8975_get_magnetism(int device, float *m)
{
    __s16 tmp[3];

    tmp[0] = i2c_smbus_read_word_data(device, 0x03);
    tmp[1] = i2c_smbus_read_word_data(device, 0x05);
    tmp[2] = i2c_smbus_read_word_data(device, 0x07);

    i2c_smbus_write_byte_data(device, 0x0A, 0x01);

    m[1] = ( (tmp[0] - min[0]) / (0.5 * (max[0] - min[0])) ) - 1.0;
    m[0] = ( (tmp[1] - min[1]) / (0.5 * (max[1] - min[1])) ) - 1.0;
    m[2] = - (( (tmp[2] - min[2]) / (0.5 * (max[2] - min[2])) ) - 1.0);
}
