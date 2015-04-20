/* mpu6050.c -- i2c accelerometer and gyroscope driver
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
#include "mpu6050.h"

int mpu6050_open(int adapter, int address)
{
    int device = i2c_open(adapter, address);

    if (device == -1)
        return -1;

    i2c_smbus_write_byte_data(device, MPU6050_PWR_MGMT_1, 0x01);
    i2c_smbus_write_byte_data(device, MPU6050_SMPLRT_DIV, 7);

    return device;
}

void mpu6050_close(int device)
{
    i2c_smbus_write_byte_data(device, MPU6050_PWR_MGMT_1, 0x64);

    i2c_close(device);
}

void mpu6050_enable_passthrough(int device)
{
    i2c_smbus_write_byte_data(device, 0x37, 0x02);
}

void mpu6050_disable_passthrough(int device)
{
    i2c_smbus_write_byte_data(device, 0x37, 0x00);
}

void mpu6050_get_acceleration(int device, float *a)
{
    __s32 tmp[3];

    tmp[0] = i2c_smbus_read_byte_data(device, 0x3B);
    tmp[1] = i2c_smbus_read_byte_data(device, 0x3D);
    tmp[2] = i2c_smbus_read_byte_data(device, 0x3F);

    for(int n=0; n<3; n++)
        a[n] = ((__s16)BYTESWAP(tmp[n])) / 32768.0;
}

void mpu6050_get_rotation(int device, float *r)
{
    __s32 tmp[3];

    tmp[0] = i2c_smbus_read_byte_data(device, 0x43);
    tmp[1] = i2c_smbus_read_byte_data(device, 0x45);
    tmp[2] = i2c_smbus_read_byte_data(device, 0x47);

    for(int n=0; n<3; n++)
        r[n] = ((__s16)BYTESWAP(tmp[n])) / 32768.0;
}
