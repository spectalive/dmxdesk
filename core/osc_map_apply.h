// A message from the console into the map's live state: a button's on, a
// fader's level (a float as 0..1, an int as 0..255), a dial's time (a float
// as 0..1 of its msRange when it has one, otherwise ms) or multiplier, a
// multiplier button pressed, a function's running. What the message named.
#ifndef OSC_MAP_APPLY_H
#define OSC_MAP_APPLY_H

#include "osc_map.h"
#include "osc_msg.h"
#include "osc_target.h"

struct osc_target osc_map_apply(struct osc_map *map, const struct osc_msg *msg);

#endif
