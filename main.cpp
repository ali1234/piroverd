/* main.cpp -- piroverd
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
#include <unistd.h>

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#include "sensors.h"
#include "outputs.h"

static Outputs *outputs;
static Sensors *sensors;

static gboolean udp_received(GSocket *sock, GIOCondition condition, gpointer data)
{
    char buf[1024];
    gsize bytes_read;
    GSocketAddress *address;
    GError *err = NULL;

    if (condition & G_IO_HUP) return FALSE;

    bytes_read = g_socket_receive_from(sock, &address, buf, sizeof(buf), NULL, &err);
    g_assert (err == NULL);

    outputs->handle_control_packet(buf);

    return TRUE;
}



static gboolean cleanup_sessions (GstRTSPServer *server)
{
    GstRTSPSessionPool *pool;

    pool = gst_rtsp_server_get_session_pool (server);
    gst_rtsp_session_pool_cleanup (pool);
    g_object_unref (pool);

    return TRUE;
}

static gboolean annotation_update (GstElement *rpicam)
{

    if(GST_OBJECT_REFCOUNT_VALUE(rpicam) == 1) return FALSE;

    char buf[100];

    sensors->update_battery();
    sensors->update_orientation();
    sensors->get_annotation(buf);

    g_print(buf);
    g_print("\n");

    gst_util_set_object_arg (G_OBJECT (rpicam), "annotation-text", buf);

    return TRUE;
}

static void media_configure (GstRTSPMediaFactory *factory, GstRTSPMedia *media,
    gpointer user_data)
{
    GstElement *element, *rpicam;

    element = gst_rtsp_media_get_element (media);
    rpicam = gst_bin_get_by_name_recurse_up (GST_BIN (element), "rpicam");

    g_timeout_add_full (G_PRIORITY_DEFAULT, 100, (GSourceFunc)annotation_update,
                          g_object_ref(rpicam), g_object_unref);

    gst_object_unref (rpicam);
    gst_object_unref (element);
}

int main(int argc, char *argv[])
{
    GMainLoop *loop;
    GstRTSPServer *server;
    GstRTSPMountPoints *mounts;
    GstRTSPMediaFactory *factory;

    GSocket *socket;
    GSource *source;
    GError *err = NULL;

    gst_init (&argc, &argv);

    sensors = new Sensors(1);
    outputs = new Outputs(1);

    loop = g_main_loop_new (NULL, FALSE);

    server = gst_rtsp_server_new ();
    mounts = gst_rtsp_server_get_mount_points (server);

    factory = gst_rtsp_media_factory_new ();
    gst_rtsp_media_factory_set_launch (factory, "( "
        "rpicamsrc name=rpicam annotation-mode=custom-text annotation-text=\"hello\" preview=false bitrate=2000000"
        " ! video/x-h264, width=960, height=540, framerate=15/1 ! h264parse ! rtph264pay name=pay0 config-interval=1"
        " )");

    g_signal_connect (factory, "media-configure", (GCallback) media_configure, NULL);

    gst_rtsp_mount_points_add_factory (mounts, "/test", factory);
    g_object_unref (mounts);

    gst_rtsp_server_attach (server, NULL); // TODO: error check

    g_timeout_add_seconds (2, (GSourceFunc)cleanup_sessions, server);

    socket = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &err);
    g_assert(err == NULL);
    g_socket_bind(socket,
        g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 5005),
        TRUE, &err);
    g_assert(err == NULL);

    source = g_socket_create_source(socket, G_IO_IN, NULL);
    g_source_set_callback(source, (GSourceFunc)udp_received, NULL, NULL);
    g_source_attach(source, NULL);
//    g_object_unref(source);
//    g_object_unref(socket);

    g_main_loop_run (loop);

    delete outputs;
    delete sensors;
}
