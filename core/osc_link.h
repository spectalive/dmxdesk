// A console reached over OSC on UDP: its map, the socket the desk sends from
// and hears replies on, and the replies a console without feedback would
// have sent, looped back. UDP has no connection: the link is up once the
// socket is open and the console's address resolves, and it says nothing
// about whether anything listens there.
#ifndef OSC_LINK_H
#define OSC_LINK_H

#include <stdint.h>
#include <sys/socket.h>

#include "osc_map.h"
#include "osc_msg.h"

#define OSC_HOST_MAX 128
#define OSC_LOOP_MAX 32
#define OSC_RETRY_MS 1500

struct osc_link_config {
    const char *surface_path;
    char host[OSC_HOST_MAX];
    int port;           // where the console listens
    int listen_port;    // where the desk hears it, or 0 for any
};

struct osc_link {
    struct osc_map map;
    char host[OSC_HOST_MAX];
    int port;
    int listen_port;
    int fd;
    struct sockaddr_storage dest;
    socklen_t dest_len;
    int snapshot_pending;
    int64_t retry_at;
    struct osc_msg loop[OSC_LOOP_MAX];
    int loop_head, loop_count;
    char reason[96];
};

#endif
