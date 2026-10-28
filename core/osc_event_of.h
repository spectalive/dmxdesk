// What a message the map has just applied means to the desk: a button's
// function running or not, the grand master's level, a dial's time and
// multiplier together, a function's state. DESK_EVENT_NONE otherwise.
#ifndef OSC_EVENT_OF_H
#define OSC_EVENT_OF_H

#include "desk_event.h"
#include "osc_map.h"
#include "osc_target.h"

struct desk_event osc_event_of(const struct osc_map *map, struct osc_target target);

#endif
