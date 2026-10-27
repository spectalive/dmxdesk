// A modal transition - the surface opening or closing, a lock or an unlock:
// every gesture under it ends, holds release, the speed cards forget their taps.
#ifndef DESK_END_GESTURES_H
#define DESK_END_GESTURES_H

#include <stdint.h>

#include "desk_app.h"

void desk_end_gestures(struct desk_app *app, int64_t now);

#endif
