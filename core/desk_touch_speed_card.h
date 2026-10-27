// On the SPEED page the content under the state row is the cards': the
// finger they captured ends there. Returns 1 when it was theirs.
#ifndef DESK_TOUCH_SPEED_CARD_H
#define DESK_TOUCH_SPEED_CARD_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"
#include "touch_event.h"

int desk_touch_speed_card(struct desk_app *app, const struct desk_platform *p,
                          const struct touch_event *e, int64_t now);

#endif
