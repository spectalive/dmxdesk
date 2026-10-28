// A dial's optional "factorAddresses", { "<multiplier>": "/address", ... },
// appended to the map's factor presses for the item at index item. NULL, or
// why it is wrong: only a dial takes it, not together with a factorAddress,
// and each key is a multiplier 0..10.
#ifndef OSC_FACTOR_PRESSES_PARSE_H
#define OSC_FACTOR_PRESSES_PARSE_H

#include <cjson/cJSON.h>

#include "osc_map.h"

const char *osc_factor_presses_parse(const cJSON *node, int item, struct osc_map *map);

#endif
