// The turn's touches. Every contact counts for the lock, even when its
// coordinates are invalid; out-of-bounds moves and releases cancel the
// gesture and never commit an action or map a fader. When the source ends,
// the holds release before the desk stops.
#ifndef DESK_TAKE_TOUCHES_H
#define DESK_TAKE_TOUCHES_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"

void desk_take_touches(struct desk_app *app, const struct desk_platform *p, int64_t now);

#endif
