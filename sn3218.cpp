/* sn3218.cpp -- i2c pwm driver
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

#include "sn3218.h"

SN3218::SN3218(int adapter, int address) : I2CDevice(adapter, address)
{
    write8(0, 1);
}

SN3218::~SN3218()
{
    ;
}

void SN3218::set_light(int light, int brightness)
{
    if (brightness > 255) brightness = 255;
    else if (brightness < 0) brightness = 0;

    write8(light+1, brightness);
    int reg = 19 + (light / 6);
    int bit = 1 << (light % 6);
    __u8 ctrl = read8(reg);
    if (brightness == 0) ctrl &= ~bit;
    else ctrl |= bit;

    write8(reg, ctrl);
    write8(22, 0);

}
