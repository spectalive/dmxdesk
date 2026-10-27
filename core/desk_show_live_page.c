#include "desk_show_live_page.h"

#include <string.h>

int desk_show_live_page(const struct show_map *map) {
    for (int i = 0; i < map->pages; i++)
        if (strcmp(map->page[i].key, "live") == 0)
            return i;
    return -1;
}
