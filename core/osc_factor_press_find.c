#include "osc_factor_press_find.h"

int osc_factor_press_find(const struct osc_map *map, int item, int factor) {
    for (int i = 0; i < map->factor_presses; i++)
        if (map->factor_press[i].item == item && map->factor_press[i].factor == factor)
            return i;
    return -1;
}
