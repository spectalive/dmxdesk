// The Qt pipe's frame: the view as one @STATE line, at most every 50 ms
// while something changes and every 500 ms regardless.
#ifndef LINUX_DESK_PUBLISH_H
#define LINUX_DESK_PUBLISH_H

#include <stdint.h>

#include "desk_app.h"
#include "linux_desk.h"

void linux_desk_publish(struct linux_desk *d, struct desk_app *app, int damaged, int64_t now);

#endif
