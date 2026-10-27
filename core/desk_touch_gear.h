// The gear: a tap opens or closes the settings surface, when the surface is
// allowed at all. Returns 1 when the contact was the gear's.
#ifndef DESK_TOUCH_GEAR_H
#define DESK_TOUCH_GEAR_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"
#include "touch_event.h"

int desk_touch_gear(struct desk_app *app, const struct desk_platform *p,
                    const struct touch_event *e, int64_t now);

#endif
