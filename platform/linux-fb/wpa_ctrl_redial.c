#include "wpa_ctrl_redial.h"
#include "wpa_ctrl_dial.h"
#include "wpa_ctrl_private.h"

int wpa_ctrl_redial(struct wpa_ctrl *c) {
    if (c->fd >= 0)
        return 0;
    if (!c->local[0])
        return -1;
    c->fd = wpa_ctrl_dial(c->path, c->local);
    return c->fd >= 0 ? 0 : -1;
}
