// A control's optional `releaseTo` as the map writes it: an object from a
// state control's widget id, as a decimal string, to a hook's widget id.
#ifndef SHOWMAP_RELEASE_TO_PARSE_H
#define SHOWMAP_RELEASE_TO_PARSE_H

#include <cjson/cJSON.h>

#include "showmap.h"

// Reads `item`'s releaseTo into c->release_to. Absent, null or not an object
// is no entries; an entry whose key is not a decimal widget id or whose
// value is not an integer widget id is skipped with a line on stderr, as is
// any past MAP_MAX_RELEASES. Never fails: the field is advisory. Whether the
// widgets are a state and a hook of the control's frame is checked once the
// whole map is known (showmap_release_to_prune).
void showmap_release_to_parse(const cJSON *item, struct map_control *c);

#endif
