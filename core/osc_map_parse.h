// A surface file into a console map:
//
//   { "osc": 1, "version": "5.2.2", "feedback": false, "grandMaster": 266,
//     "items": [ { "id": 4, "kind": "toggle", "group": 2, "function": 17,
//                  "address": "/vibra/4" }, ... ],
//     "functions": [ { "id": 17, "address": "/vibra/function/17" } ] }
//
// An item also takes "disabled", "on", "level", "maxHoldMs", "priority",
// "ms", "factor", "maxMs" and, on a dial, "factorAddress": the same fields
// desk_surface carries. "version" is matched against the show map's console
// version when it is set. "grandMaster" names a level item.
// 0, or -1 with why in why[]; out is empty on failure.
#ifndef OSC_MAP_PARSE_H
#define OSC_MAP_PARSE_H

#include <stddef.h>

#include "osc_map.h"

int osc_map_parse(const char *json, size_t len, struct osc_map *out, char *why, size_t cap);

#endif
