// A surface file's "items" into the map's surface and addresses, which it
// allocates. NULL, or why the list is wrong; the ids must rise strictly,
// which is the order desk_surface_find searches. An item's "feedback":
// false marks a control its console does not report, which the desk then
// echoes itself, as QLC+ does not report a speed dial.
#ifndef OSC_MAP_PARSE_ITEMS_H
#define OSC_MAP_PARSE_ITEMS_H

#include <cjson/cJSON.h>

#include "osc_map.h"

const char *osc_map_parse_items(const cJSON *items, struct osc_map *map);

#endif
