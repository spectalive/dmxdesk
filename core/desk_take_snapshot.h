// A fresh console from the master: the model is rebuilt against it, and a
// finger on a hit has its release sent first, from the model that knows it,
// or the output stays on.
#ifndef DESK_TAKE_SNAPSHOT_H
#define DESK_TAKE_SNAPSHOT_H

#include <stdint.h>

#include "desk_app.h"

void desk_take_snapshot(struct desk_app *app, int64_t now);

#endif
