// A hold or the tempo card: pressed on contact, owned per finger, never a
// model capture. A hold fires only unlocked and linked. Returns 1 when the
// contact landed on one.
#ifndef DESK_TOUCH_PRESS_H
#define DESK_TOUCH_PRESS_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"
#include "touch_event.h"

int desk_touch_press(struct desk_app *app, const struct desk_platform *p,
                     const struct touch_event *e, int64_t now);

#endif
