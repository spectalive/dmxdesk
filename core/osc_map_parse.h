// A surface file into a console map:
//
//   { "osc": 1, "version": "5.2.2", "feedback": false, "grandMaster": 266,
//     "items": [ { "id": 4, "kind": "toggle", "group": 2, "function": 17,
//                  "address": "/vibra/4" }, ... ],
//     "functions": [ { "id": 17, "address": "/vibra/function/17" } ] }
//
// An item also takes "disabled", "on", "level", "maxHoldMs", "priority",
// "ms", "factor", "maxMs": the same fields desk_surface carries. A dial
// also takes how its console wants its time and multiplier: "msRange":
// [min, max] for the time as 0..1 of that range (otherwise an int in ms),
// and either "factorAddress" for the multiplier as an int or
// "factorAddresses": { "6": "/x/1x", "7": "/x/2x" } for a button per
// multiplier, which is how QLC+ takes it. "feedback": false on an item
// marks a control a reporting console leaves silent. "version" is matched against the show map's console
// version when it is set. "grandMaster" names a level item.
// 0, or -1 with why in why[]; out is empty on failure.
#ifndef OSC_MAP_PARSE_H
#define OSC_MAP_PARSE_H

#include <stddef.h>

#include "osc_map.h"

int osc_map_parse(const char *json, size_t len, struct osc_map *out, char *why, size_t cap);

#endif
