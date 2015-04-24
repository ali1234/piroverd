/* pca9685.cpp -- i2c pwm driver
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

PCA9685::PCA9685(int adapter, int address) : I2CDevice(adapter, address)
{
    write8(0xfa, 0x00);
    write8(0xfb, 0x00);
    write8(0xfc, 0x00);
    write8(0xfd, 0x00);

    write8(0x01, 0x04);
    write8(0x00, 0x01);
    g_usleep(500);

    write8(0x00, 0x01);
    g_usleep(500);

    write8(0x00, 0x11);
    write8(0xFE, 0x03);

    write8(0x00, 0x01);
    g_usleep(500);

    write8(0x00, 0x81);
    g_usleep(500);
}

PCA9685::~PCA9685()
{
    write8(0xfd, 0x10);
    write8(0x00, 0x10);
}

void PCA9685::set_pwm(int pin, int on, int off)
{
    if(on > 4095) on = 4095;
    if(off > 4095) off = 4095;

    write8(PCA9685_LED0_ON_L+(4*pin), on&0xff);
    write8(PCA9685_LED0_ON_H+(4*pin), on>>8);
    write8(PCA9685_LED0_OFF_L+(4*pin), off&0xff);
    write8(PCA9685_LED0_OFF_H+(4*pin), off>>8);
}

void PCA9685::set_pin(int pin, int value)
{
    if (value)
        set_pwm(pin, 4096, 0);
    else
        set_pwm(pin, 0, 4096);
}

void PCA9685::set_motor_mode(int motor, int mode)
{
    /* Always turn OFF before turning ON to prevent SHORTS. */
    if(mode > 0) {
        set_pin(motors[motor][IN2], 0);
        set_pin(motors[motor][IN1], 1);
    } else if (mode < 0) {
        set_pin(motors[motor][IN1], 0);
        set_pin(motors[motor][IN2], 1);
    } else {
        set_pin(motors[motor][IN1], 0);
        set_pin(motors[motor][IN2], 0);
    }
}

void PCA9685::set_motor_speed(int motor, int speed)
{
    set_pwm(motors[motor][PWM], 0, speed);
}
