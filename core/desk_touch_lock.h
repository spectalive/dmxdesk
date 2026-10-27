// The lock target: a tap locks, a long hold unlocks; either way every
// gesture under it ends.
#ifndef DESK_TOUCH_LOCK_H
#define DESK_TOUCH_LOCK_H

#include <stdint.h>

#include "desk_app.h"
#include "touch_event.h"

void desk_touch_lock(struct desk_app *app, const struct touch_event *e, int64_t now);

#endif
