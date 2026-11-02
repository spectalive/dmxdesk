#include "wpa_ctrl.h"
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include "wpa_ctrl_private.h"
#include "wpa_ctrl_reattach.h"
#include "wpa_ctrl_redial.h"

int wpa_ctrl_begin(struct wpa_ctrl *c, const char *cmd) {
    if (!c || !cmd || !cmd[0])
        return -1;
    // An abandon whose redial failed (the daemon restarting) left no socket.
    // Each request dials once more, so the channel comes back with the
    // daemon instead of staying dead for the rest of the run.
    int redialled = c->fd < 0;
    if (wpa_ctrl_redial(c) != 0)
        return -1;
    // Having to dial means the daemon restarted, and a new daemon never
    // heard our ATTACH: without this, scan results stop arriving for good.
    if (redialled || c->event_fd < 0)
        wpa_ctrl_reattach(c);
    if (c->pending) {
        errno = EBUSY;
        return -1;
    }
    size_t len = strlen(cmd);
    if (send(c->fd, cmd, len, 0) != (ssize_t)len) {
        // A daemon that restarted while nothing was pending leaves the socket
        // connected to its dead predecessor, so every send fails until it is
        // dialled again. One fresh dial, one more send; a daemon still gone
        // fails here and the next request dials again.
        wpa_ctrl_abandon(c);
        if (c->fd < 0)
            return -1;
        wpa_ctrl_reattach(c);
        if (send(c->fd, cmd, len, 0) != (ssize_t)len)
            return -1;
    }
    c->pending = 1;
    return 0;
}
