#include "wpa_ctrl_reattach.h"
#include <sys/socket.h>
#include <unistd.h>
#include "wpa_ctrl_dial.h"
#include "wpa_ctrl_private.h"

int wpa_ctrl_reattach(struct wpa_ctrl *c) {
    if (c->event_fd >= 0)
        close(c->event_fd);
    c->attach_pending = 0;
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
