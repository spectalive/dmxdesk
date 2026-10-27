// What the settings surface asked for. Closing and a new master are the
// desk's; scanning, joining, finding and the backlight go to the platform.
#ifndef DESK_TAKE_SETUP_ACTION_H
#define DESK_TAKE_SETUP_ACTION_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"

void desk_take_setup_action(struct desk_app *app, const struct desk_platform *p,
                            const struct setup_action *act, int64_t now);

#endif
