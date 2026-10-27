// One touch, offered in turn to whatever may own it: the gear, the settings
// surface, the speed cards, a held control, a hold or the tempo card, then
// the surface's own targets and tiles.
#ifndef DESK_ROUTE_TOUCH_H
#define DESK_ROUTE_TOUCH_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"
#include "touch_event.h"

void desk_route_touch(struct desk_app *app, const struct desk_platform *p,
                      const struct touch_event *e, int64_t now);

#endif
