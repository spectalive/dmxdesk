#include "desk_family_icon.h"

#include <string.h>

#include "icon_data.h"
#include "icon_glyph.h"

int desk_family_icon(const struct show_map *map, int page) {
    static const struct { const char *key; enum icon_id icon; } known[] = {
        { "color", ICON_PALETTE }, { "pixels", ICON_GRID }, { "heads", ICON_MOVING_HEAD },
        { "gobos", ICON_GOBO }, { "prism", ICON_PRISM }, { "control", ICON_SLIDERS },
    };
    if (page < 0 || page >= map->pages)
        return -1;
    for (size_t i = 0; i < sizeof known / sizeof known[0]; i++)
        if (strcmp(map->page[page].key, known[i].key) == 0)
            return known[i].icon;
    for (int i = 0; i < map->count; i++)
        if (map->control[i].page == page && icon_glyph(map->control[i].icon) >= 0)
            return icon_glyph(map->control[i].icon);
    return -1;
}
