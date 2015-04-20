/* net.c -- receive controls over udp
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

#include "pca9685.h"
#include "sn3218.h"
#include "net.h"

static GSocket *socket;
static GSource *source;
static int pca9685;
static int sn3218;

static gboolean udp_received(GSocket *sock, GIOCondition condition, gpointer data)
{
    char buf[1024];
    gsize bytes_read;
    GSocketAddress *address;
    GError *err = NULL;

    if (condition & G_IO_HUP) return FALSE;

    bytes_read = g_socket_receive_from(sock, &address, buf, sizeof(buf), NULL, &err);
    g_assert (err == NULL);

//    g_print("%d\n", bytes_read);

    for (int n=0; n<4; n++)
    {
        unsigned short m = (buf[2*n]<<8)|buf[(2*n)+1];
        pca9685_set_motor_speed(pca9685, n, m&0x7fff);
        if(m&0x7fff == 0) {
            pca9685_set_motor_mode(pca9685, n, 0);
        } else if (m&0x8000) {
            pca9685_set_motor_mode(pca9685, n, -1);
        } else {
            pca9685_set_motor_mode(pca9685, n, 1);
        }
    }

    return TRUE;
}

static gboolean test_lights(gpointer unused, gboolean ignored)
{
    static int l = 17;

    sn3218_set_light(sn3218, l, 0);

    l += 1;
    if (l == 5) l = 13;
    l = l % 18;

    sn3218_set_light(sn3218, l, 255);

    return TRUE;
}

void net_start(void)
{
    GError *err = NULL;

    pca9685 = pca9685_open(1, 0x60);
    sn3218 = sn3218_open(1, 0x54);

    socket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &err);
    g_assert(err == NULL);

    g_socket_bind(socket,
        g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 5005),
        TRUE, &err);
    g_assert(err == NULL);

    source = g_socket_create_source(socket, G_IO_IN, NULL);
    g_source_set_callback(source, (GSourceFunc)udp_received, NULL, NULL);
    g_source_attach(source, NULL);

    g_timeout_add(150, (GSourceFunc)test_lights, NULL);
}

void net_stop(void)
{
    g_object_unref(socket);

    pca9685_close(pca9685);
}
