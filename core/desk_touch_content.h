// A contact on the page's tiles: the model's gesture, the speed cards on
// the SPEED page, and what the gesture sends.
#ifndef DESK_TOUCH_CONTENT_H
#define DESK_TOUCH_CONTENT_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"
#include "touch_event.h"

void desk_touch_content(struct desk_app *app, const struct desk_platform *p,
                        const struct touch_event *e, int64_t now);

#endif
