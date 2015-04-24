/* ak8975.cpp -- i2c magnetometer driver
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

#include <glib.h>

#include "ak8975.h"

int min[3] = { -254, -97, -159 };
int max[3] = { 22, 191, 130 };

AK8975::AK8975(int adapter, int address) : I2CDevice(adapter, address)
{
    write8(0x0A, 0x01);
}

AK8975::~AK8975()
{
    write8(0x0A, 0x00);
}

void AK8975::get_magnetism(float *m)
{
    __s16 tmp[3];

    tmp[0] = read8(0x03)|(read8(0x04) << 8);
    tmp[1] = read8(0x05)|(read8(0x06) << 8);
    tmp[2] = read8(0x07)|(read8(0x08) << 8);

    write8(0x0A, 0x01);

    m[1] = ( (tmp[0] - min[0]) / (0.5 * (max[0] - min[0])) ) - 1.0;
    m[0] = ( (tmp[1] - min[1]) / (0.5 * (max[1] - min[1])) ) - 1.0;
    m[2] = - (( (tmp[2] - min[2]) / (0.5 * (max[2] - min[2])) ) - 1.0);
}
