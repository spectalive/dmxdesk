#include "wpa_ctrl.h"
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include "wpa_ctrl_private.h"
#include "wpa_ctrl_redial.h"

int wpa_ctrl_begin(struct wpa_ctrl *c, const char *cmd) {
    if (!c || !cmd || !cmd[0])
        return -1;
    // An abandon whose redial failed (the daemon restarting) left no socket.
    // Each request dials once more, so the channel comes back with the
    // daemon instead of staying dead for the rest of the run.
    if (wpa_ctrl_redial(c) != 0)
        return -1;
    if (c->pending) {
        errno = EBUSY;
        return -1;
    }
    size_t len = strlen(cmd);
    if (send(c->fd, cmd, len, 0) != (ssize_t)len)
        return -1;
    c->pending = 1;
    return 0;
}
