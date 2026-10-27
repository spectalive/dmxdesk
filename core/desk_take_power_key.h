// The power key's press: the lock blanks or wakes, and every finger's claim
// goes with the display - the desk's own slot bookkeeping too, or a reused
// slot would inherit a gesture.
#ifndef DESK_TAKE_POWER_KEY_H
#define DESK_TAKE_POWER_KEY_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"

void desk_take_power_key(struct desk_app *app, const struct desk_platform *p, int64_t now);

#endif
