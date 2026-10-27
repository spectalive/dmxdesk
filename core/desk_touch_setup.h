// The settings surface owns one contact, the first that lands on it; a
// second finger on the sheet is swallowed so it can neither steal nor end
// the first one's gesture. The master column beside the sheet stays the
// desk's. Returns 1 when the contact was the surface's.
#ifndef DESK_TOUCH_SETUP_H
#define DESK_TOUCH_SETUP_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"
#include "touch_event.h"

int desk_touch_setup(struct desk_app *app, const struct desk_platform *p,
                     const struct touch_event *e, int64_t now);

#endif
