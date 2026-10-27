// Scan, join, find and the backlight, as the settings surface asked.
#ifndef LINUX_DESK_SETUP_ACTION_H
#define LINUX_DESK_SETUP_ACTION_H

#include <stdint.h>

#include "desk_app.h"

void linux_desk_setup_action(void *ctx, struct desk_app *app, const struct setup_action *act,
                             int64_t now);

#endif
