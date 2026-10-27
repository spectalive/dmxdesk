// Holds: the cap, the link, the owed releases, the tiles' countdown. A
// hold whose release is still owed shows it: the output may be on.
#ifndef DESK_TICK_HOLDS_H
#define DESK_TICK_HOLDS_H

#include <stdint.h>

#include "desk_app.h"

void desk_tick_holds(struct desk_app *app, int64_t now);

#endif
