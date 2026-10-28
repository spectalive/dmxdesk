#include "linux_desk_wait.h"

#include <unistd.h>

#include "linux_desk.h"
#include "pollfd_watch.h"

void linux_desk_wait(void *ctx, const struct pollfd *master, int count, int timeout_ms) {
    struct linux_desk *d = ctx;
    struct pollfd fds[8];
    int n = 0, touch_slot = -1, power_slot = -1;
    if (d->qt_pipe) {
        touch_slot = n;
        pollfd_watch(fds, &n, STDIN_FILENO);
        fds[touch_slot].events = POLLIN | POLLHUP;
    }
    if (d->touch) {
        touch_slot = n;
        pollfd_watch(fds, &n, d->touch_fd);
    }
    if (d->power_fd >= 0) {
        power_slot = n;
        pollfd_watch(fds, &n, d->power_fd);
    }
    if (d->wpa) {
        pollfd_watch(fds, &n, wpa_ctrl_event_fd(d->wpa));
        pollfd_watch(fds, &n, wpa_ctrl_request_fd(d->wpa));
    }
    for (int i = 0; i < count && i < 2; i++)
        fds[n++] = master[i];
    poll(fds, (nfds_t)n, timeout_ms);
    d->touch_ready = touch_slot >= 0 && (fds[touch_slot].revents & POLLIN);
    d->power_ready = power_slot >= 0 && (fds[power_slot].revents & POLLIN);
}
