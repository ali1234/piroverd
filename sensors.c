/* sensors.c -- hardware sensor control
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

#include <glib.h>

#include "max17043.h"
#include "mpu6050.h"
#include "ak8975.h"

#include "sensors.h"

static int max17043;
static int mpu6050;
static int ak8975;

static float acceleration[3];
static float rotation[3];
static float magnetism[3];
static float voltage, percent;

static GMutex sensor_mutex;

static gboolean slow_update(gpointer unused, gboolean ignored)
{
    g_mutex_lock (&sensor_mutex);

    voltage = max17043_voltage(max17043);
    percent = max17043_percent(max17043);

    g_print("%.2fV %.0f%%\n", voltage, percent);

    g_mutex_unlock (&sensor_mutex);

    return TRUE;
}

static gboolean fast_update(gpointer unused, gboolean ignored)
{
    g_mutex_lock (&sensor_mutex);

    mpu6050_get_acceleration(mpu6050, acceleration);
    mpu6050_get_rotation(mpu6050, rotation);
    ak8975_get_magnetism(ak8975, magnetism);

    g_mutex_unlock (&sensor_mutex);

    return TRUE;
}

void sensors_start(int adapter)
{
    max17043 = max17043_open(adapter, 0x36);
    mpu6050 = mpu6050_open(adapter, 0x68);
    mpu6050_enable_passthrough(mpu6050);
    ak8975 = ak8975_open(adapter, 0x0C);

    slow_update(NULL, TRUE);
    fast_update(NULL, TRUE);

    g_timeout_add_seconds(10, (GSourceFunc)slow_update, NULL);
    g_timeout_add(100, (GSourceFunc)fast_update, NULL);

}

void sensors_stop(void)
{
    ak8975_close(ak8975);
    mpu6050_close(mpu6050);
    max17043_close(max17043);
}

void sensors_get_annotation(char *buf)
{
    g_mutex_lock (&sensor_mutex);

    sprintf(buf, "%.2fV %.0f%%", voltage, percent);

    g_mutex_unlock (&sensor_mutex);
}
