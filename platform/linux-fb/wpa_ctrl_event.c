#include "wpa_ctrl.h"
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include "wpa_ctrl_private.h"

int wpa_ctrl_event(struct wpa_ctrl *c, char *buf, size_t cap) {
    if (!c || c->event_fd < 0 || !buf || cap < 2)
        return -1;
    ssize_t n = recv(c->event_fd, buf, cap - 1, 0);
    if (n < 0)
        return errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR ? 0 : -1;
    buf[n] = '\0';
    // A reattach's ATTACH is answered here; events carry a "<n>" prefix and
    // a reply does not, so the reply is dropped rather than read as one.
    if (c->attach_pending && buf[0] != '<') {
        c->attach_pending = 0;
        return wpa_ctrl_event(c, buf, cap);
    }
    // "<3>CTRL-EVENT-...": the priority prefix is the daemon's, not ours.
    if (buf[0] == '<') {
        char *end = strchr(buf, '>');
        if (end)
            memmove(buf, end + 1, strlen(end + 1) + 1);
    }
    // The daemon ends its events with a trailing space, and some with a
    // newline; neither is part of the event.
    size_t len = strlen(buf);
    while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r' || buf[len - 1] == ' '))
        buf[--len] = '\0';
    return 1;
}
