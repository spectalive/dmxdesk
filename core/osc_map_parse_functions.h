// A surface file's optional "functions" into the map, which it allocates.
// NULL, or why the list is wrong.
#ifndef OSC_MAP_PARSE_FUNCTIONS_H
#define OSC_MAP_PARSE_FUNCTIONS_H

#include <cjson/cJSON.h>

#include "osc_map.h"

const char *osc_map_parse_functions(const cJSON *functions, struct osc_map *map);

#endif
