// The section keyed `key` on map page `page`, or NULL when the page has none
// (or `page` is -1).
#ifndef DESK_SHOW_SECTION_H
#define DESK_SHOW_SECTION_H

#include "showmap.h"

const struct map_section *desk_show_section(const struct show_map *map, int page, const char *key);

#endif
