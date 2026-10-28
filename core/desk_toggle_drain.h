// A toggle whose 200 ms slot has come: sent now, or dropped if the link went
// down while it waited in the queue.
#ifndef DESK_TOGGLE_DRAIN_H
#define DESK_TOGGLE_DRAIN_H

#include "desk_toggle_queue.h"
#include "desk_console.h"

#include <stdint.h>

void desk_toggle_drain(const struct desk_console *console, struct desk_toggle_queue *toggles, int64_t now);

#endif
