/* max17043.c -- i2c single cell lipo fuel gauge driver
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

#include <math.h>

#include <linux/i2c-dev.h>

#include "i2c.h"
#include "max17043.h"

int max17043_open(int adapter, int address)
{
    int device = i2c_open(adapter, address);

    if (device == -1)
        return -1;

    __s32 config = i2c_smbus_read_word_data(device, MAX17043_CONFIG);

    if(config == -1)
        return -1;

    config &= ~MAX17043_CONFIG_SLEEP;
    i2c_smbus_write_word_data(device, MAX17043_CONFIG, config);

    return device;
}

void max17043_close(int device)
{
    __s32 config = i2c_smbus_read_word_data(device, MAX17043_CONFIG);

    if(config != -1) {
        config |= MAX17043_CONFIG_SLEEP;
        i2c_smbus_write_word_data(device, MAX17043_CONFIG, config);
    }

    i2c_close(device);
}



float max17043_voltage(int device)
{
    __s32 vcell = i2c_smbus_read_word_data(device, MAX17043_VCELL);

    if (vcell == -1)
        return NAN;

    return (BYTESWAP(vcell) >> 4) * 0.00125;
}

float max17043_percent(int device)
{
    __s32 soc = i2c_smbus_read_word_data(device, MAX17043_SOC);

    if (soc == -1)
        return NAN;

    return BYTESWAP(soc) / 256.0;
}

