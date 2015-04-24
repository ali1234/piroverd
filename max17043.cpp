/* max17043.cpp -- i2c single cell lipo fuel gauge driver
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

#include "max17043.h"

MAX17043::MAX17043(int adapter, int address) : I2CDevice(adapter, address)
{

    __u16 config = read16(MAX17043_CONFIG);
    config &= ~MAX17043_CONFIG_SLEEP;
    write16(MAX17043_CONFIG, config);
}

MAX17043::~MAX17043()
{
    __u16 config = read16(MAX17043_CONFIG);
    config |= MAX17043_CONFIG_SLEEP;
    write16(MAX17043_CONFIG, config);
}



float MAX17043::get_voltage(void)
{
    return (BYTESWAP(read16(MAX17043_VCELL)) >> 4) * 0.00125;
}

float MAX17043::get_percent(void)
{
    return BYTESWAP(read16(MAX17043_SOC)) / 256.0;
}

