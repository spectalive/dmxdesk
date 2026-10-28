#include "osc_map_target.h"

#include <string.h>

struct osc_target osc_map_target(const struct osc_map *map, const char *address) {
    struct osc_target t = { OSC_TARGET_NONE, -1 };
    if (!address[0])
        return t;
    for (int i = 0; i < map->surface.count; i++) {
        if (!strcmp(map->address[i], address))
            return (struct osc_target){ OSC_TARGET_ITEM, i };
        if (!strcmp(map->factor_address[i], address))
            return (struct osc_target){ OSC_TARGET_FACTOR, i };
    }
    for (int i = 0; i < map->factor_presses; i++)
        if (!strcmp(map->factor_press[i].address, address))
            return (struct osc_target){ OSC_TARGET_FACTOR_PRESS, i };
    for (int i = 0; i < map->functions; i++)
        if (!strcmp(map->function[i].address, address))
            return (struct osc_target){ OSC_TARGET_FUNCTION, i };
    return t;
}
