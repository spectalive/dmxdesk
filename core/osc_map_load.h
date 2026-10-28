// A surface file from disk into a console map. 0, or -1 with why in why[].
#ifndef OSC_MAP_LOAD_H
#define OSC_MAP_LOAD_H

#include <stddef.h>

#include "osc_map.h"

int osc_map_load(const char *path, struct osc_map *out, char *why, size_t cap);

#endif
