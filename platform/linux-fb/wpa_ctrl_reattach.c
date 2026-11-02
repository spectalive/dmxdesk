#include "wpa_ctrl_reattach.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "wpa_ctrl_dial.h"
#include "wpa_ctrl_private.h"

int wpa_ctrl_reattach(struct wpa_ctrl *c) {
    if (c->event_fd >= 0) {
        // A daemon still running forgets the old address rather than send
        // every event twice; to a dead one the send just fails.
        send(c->event_fd, "DETACH", 6, MSG_DONTWAIT);
        close(c->event_fd);
        c->event_fd = -1;
    }
    if (c->local_event[0])
        unlink(c->local_event);
    c->attach_pending = 0;
    // A fresh address each time, so the old socket's DETACH reply can never
    // reach the new one and be taken for the ATTACH's.
    c->event_generation++;
    int n = snprintf(c->local_event, sizeof c->local_event, "%s/dmxdesk-ctrl-%d-ev-%d",
                     c->directory, (int)getpid(), c->event_generation);
    if (n <= 0 || (size_t)n >= sizeof c->local_event) {
        c->local_event[0] = '\0';
        return -1;
    }
    c->event_fd = wpa_ctrl_dial(c->path, c->local_event);
    if (c->event_fd < 0)
        return -1;
    if (send(c->event_fd, "ATTACH", 6, 0) != 6) {
        close(c->event_fd);
        c->event_fd = -1;
        return -1;
    }
    c->attach_pending = 1;
    return 0;
}
