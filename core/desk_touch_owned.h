// A finger that already holds a hold, a burst or the tempo card: only its
// release matters, and a hold's release goes out. Returns 1 when owned.
#ifndef DESK_TOUCH_OWNED_H
#define DESK_TOUCH_OWNED_H

#include <stdint.h>

#include "desk_app.h"
#include "touch_event.h"

int desk_touch_owned(struct desk_app *app, const struct touch_event *e, int64_t now);

#endif
