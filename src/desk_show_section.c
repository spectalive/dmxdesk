#include "desk_show_section.h"

#include <string.h>

const struct map_section *desk_show_section(const struct show_map *map, int page, const char *key) {
    if (page < 0)
        return NULL;
    for (int s = 0; s < map->page[page].sections; s++)
        if (strcmp(map->page[page].section[s].key, key) == 0)
            return &map->page[page].section[s];
    return NULL;
}
