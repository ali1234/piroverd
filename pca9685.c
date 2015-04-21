/* pca9685.c -- i2c pwm driver
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
#include <linux/i2c-dev.h>

#include "i2c.h"
#include "pca9685.h"

static unsigned char motors[4][3] = {
    {8, 9, 10},
    {13, 12, 11},
    {2, 3, 4},
    {7, 6, 5}
};

#define PWM 0
#define IN2 1
#define IN1 2

int pca9685_open(int adapter, int address)
{

    int device = i2c_open(adapter, address);

    if (device == -1)
        return -1;

    i2c_smbus_write_byte_data(device, 0xfa, 0x00);
    i2c_smbus_write_byte_data(device, 0xfb, 0x00);
    i2c_smbus_write_byte_data(device, 0xfc, 0x00);
    i2c_smbus_write_byte_data(device, 0xfd, 0x00);

    i2c_smbus_write_byte_data(device, 0x01, 0x04);
    i2c_smbus_write_byte_data(device, 0x00, 0x01);
    g_usleep(500);

    i2c_smbus_write_byte_data(device, 0x00, 0x01);
    g_usleep(500);

    i2c_smbus_write_byte_data(device, 0x00, 0x11);
    i2c_smbus_write_byte_data(device, 0xFE, 0x03);

    i2c_smbus_write_byte_data(device, 0x00, 0x01);
    g_usleep(500);

    i2c_smbus_write_byte_data(device, 0x00, 0x81);
    g_usleep(500);

    return device;
}

void pca9685_close(int device)
{
    i2c_smbus_write_byte_data(device, 0xfd, 0x10);
    i2c_smbus_write_byte_data(device, 0x00, 0x10);

    i2c_close(device);
}

void pca9685_set_pwm(int device, int pin, int on, int off)
{
    if(on > 4095) on = 4095;
    if(off > 4095) off = 4095;

    i2c_smbus_write_byte_data(device, PCA9685_LED0_ON_L+(4*pin), on&0xff);
    i2c_smbus_write_byte_data(device, PCA9685_LED0_ON_H+(4*pin), on>>8);
    i2c_smbus_write_byte_data(device, PCA9685_LED0_OFF_L+(4*pin), off&0xff);
    i2c_smbus_write_byte_data(device, PCA9685_LED0_OFF_H+(4*pin), off>>8);
}

void pca9685_set_pin(int device, int pin, int value)
{
    if (value)
        pca9685_set_pwm(device, pin, 4096, 0);
    else
        pca9685_set_pwm(device, pin, 0, 4096);
}

void pca9685_set_motor_mode(int device, int motor, int mode)
{
    /* Always turn OFF before turning ON to prevent SHORTS. */
    if(mode > 0) {
        pca9685_set_pin(device, motors[motor][IN2], 0);
        pca9685_set_pin(device, motors[motor][IN1], 1);
    } else if (mode < 0) {
        pca9685_set_pin(device, motors[motor][IN1], 0);
        pca9685_set_pin(device, motors[motor][IN2], 1);
    } else {
        pca9685_set_pin(device, motors[motor][IN1], 0);
        pca9685_set_pin(device, motors[motor][IN2], 0);
    }
}

void pca9685_set_motor_speed(int device, int motor, int speed)
{
    pca9685_set_pwm(device, motors[motor][PWM], 0, speed);
}
