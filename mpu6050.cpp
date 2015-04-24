/* mpu6050.cpp -- i2c accelerometer and gyroscope driver
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

#include "mpu6050.h"

MPU6050::MPU6050(int adapter, int address) : I2CDevice(adapter, address)
{
    write8(MPU6050_PWR_MGMT_1, 0x01);
    write8(MPU6050_SMPLRT_DIV, 7);
}

MPU6050::~MPU6050()
{
    write8(MPU6050_PWR_MGMT_1, 0x64);
}

void MPU6050::enable_passthrough()
{
    write8(0x37, 0x02);
}

void MPU6050::disable_passthrough()
{
    write8(0x37, 0x00);
}

void MPU6050::get_acceleration(float *a)
{
    __s16 tmp[3];

    tmp[0] = (read8(0x3B)<<8)|(read8(0x3C));
    tmp[1] = (read8(0x3D)<<8)|(read8(0x3E));
    tmp[2] = (read8(0x3F)<<8)|(read8(0x40));

    for(int n=0; n<3; n++)
        a[n] = ((tmp[n])) / 32768.0;
}

void MPU6050::get_rotation(float *r)
{
    __s16 tmp[3];

    tmp[0] = (read8(0x43)<<8)|(read8(0x44));
    tmp[1] = (read8(0x45)<<8)|(read8(0x46));
    tmp[2] = (read8(0x47)<<8)|(read8(0x48));

    for(int n=0; n<3; n++)
        r[n] = ((tmp[n])) / 32768.0;
}
