// Touches from the Qt pipe on stdin, or from the evdev node when the wait
// saw it readable.
#ifndef LINUX_DESK_TOUCHES_H
#define LINUX_DESK_TOUCHES_H

#include "touch_event.h"

int linux_desk_touches(void *ctx, struct touch_event *events, int cap);

#endif
