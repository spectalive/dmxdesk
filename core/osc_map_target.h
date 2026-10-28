// What an incoming address names in the map: a control's address, a dial's
// multiplier, a function's address, or nothing.
#ifndef OSC_MAP_TARGET_H
#define OSC_MAP_TARGET_H

#include "osc_map.h"
#include "osc_target.h"

struct osc_target osc_map_target(const struct osc_map *map, const char *address);

#endif
