// The bar's status, read once a second; the model is told every time and
// repaints only what changed.
#ifndef DESK_TICK_STATUS_H
#define DESK_TICK_STATUS_H

#include <stdint.h>

#include "desk_app.h"
#include "desk_platform.h"

#define DESK_STATUS_MS 1000

void desk_tick_status(struct desk_app *app, const struct desk_platform *p, int64_t now);

#endif
