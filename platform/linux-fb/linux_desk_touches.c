#include "linux_desk_touches.h"

#include <unistd.h>

#include "linux_desk.h"

int linux_desk_touches(void *ctx, struct touch_event *events, int cap) {
    struct linux_desk *d = ctx;
    if (d->qt_pipe)
        return desk_pipe_read(&d->pipe_reader, STDIN_FILENO, events, cap);
    if (d->touch && d->touch_ready)
        return touch_input_read_fd(d->touch, d->touch_fd, events, cap);
    return 0;
}
