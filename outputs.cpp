/* outputs.cpp -- handle udp control packets
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
#include <gio/gio.h>

#include "outputs.h"

Outputs::Outputs(int adapter)
{
    pca9685 = new PCA9685(adapter, 0x60);
    sn3218 = new SN3218(adapter, 0x54);
}

Outputs::~Outputs()
{
    delete pca9685;
    delete sn3218;
}

void Outputs::handle_control_packet(char *buf)
{
    for (int n=0; n<4; n++)
    {
        unsigned short m = (buf[2*n]<<8)|buf[(2*n)+1];
        pca9685->set_motor_speed(n, m&0x7fff);
        if(m&0x7fff == 0) {
            pca9685->set_motor_mode(n, 0);
        } else if (m&0x8000) {
            pca9685->set_motor_mode(n, -1);
        } else {
            pca9685->set_motor_mode(n, 1);
        }
    }
}
