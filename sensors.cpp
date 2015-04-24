/* sensors.cpp -- hardware sensor control
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

#include <stdio.h>
#include <math.h>

#include <glib.h>

#include "sensors.h"

#include "mathfunc.h"

Sensors::Sensors(int adapter)
{
    max17043 = new MAX17043(adapter, 0x36);
    mpu6050 = new MPU6050(adapter, 0x68);
    mpu6050->enable_passthrough();
    ak8975 = new AK8975(adapter, 0x0C);
}

Sensors::~Sensors()
{
    delete ak8975;
    delete mpu6050;
    delete max17043;
}

void Sensors::update_battery(void)
{
//    g_mutex_lock (&mutex);

    voltage = max17043->get_voltage();
    percent = max17043->get_percent();

//    g_mutex_unlock (&mutex);
}

void Sensors::update_orientation(void)
{
    float forward[3] = {0, 1, 0};
    float north[3];
    float tmp[3];

//    g_mutex_lock(&mutex);

    mpu6050->get_acceleration(acceleration);
    mpu6050->get_rotation(rotation);
    ak8975->get_magnetism(magnetism);

    pitch = 180.0*unsigned_angle_between(forward, acceleration)/3.141;
    pitch -= 90;

    projection(magnetism, acceleration, north);
    subtract(magnetism, north, north);
    normalise(north);

    projection(forward, acceleration, tmp);
    subtract(forward, tmp, tmp);

    heading = 180.0*angle_between(north, tmp, acceleration)/3.14159;
    heading += 180;

//    g_mutex_unlock(&mutex);
}

void Sensors::get_annotation(char *buf)
{
//    g_mutex_lock(&mutex);

    sprintf(buf, "%4.2fV %3.0f%% | %3.0fH %3.0fP", voltage, percent, heading, pitch);

//    g_mutex_unlock(&mutex);
}
