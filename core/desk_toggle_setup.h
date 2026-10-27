// The gear's tap: opens the settings surface, or closes it.
#ifndef DESK_TOGGLE_SETUP_H
#define DESK_TOGGLE_SETUP_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"

void desk_toggle_setup(struct desk_app *app, const struct desk_platform *p, int64_t now);

#endif
