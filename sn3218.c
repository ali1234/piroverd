/* sn3218.c -- i2c pwm driver
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
#include "sn3218.h"

int sn3218_open(int adapter, int address)
{
    int device = i2c_open(adapter, address);

    if (device == -1)
        return -1;

    i2c_smbus_write_byte_data(device, 0, 1);

    return device;
}

void sn3218_close(int device)
{
    i2c_close(device);
}

void sn3218_set_light(int device, int light, int brightness)
{
    if (brightness > 255) brightness = 255;
    else if (brightness < 0) brightness = 0;

    i2c_smbus_write_byte_data(device, light+1, brightness);
    int reg = 19 + (light / 6);
    int bit = 1 << (light % 6);
    __u8 ctrl = i2c_smbus_read_byte_data(device, reg);
    if (brightness == 0) ctrl &= ~bit;
    else ctrl |= bit;

    i2c_smbus_write_byte_data(device, reg, ctrl);
    i2c_smbus_write_byte_data(device, 22, 0);

}
