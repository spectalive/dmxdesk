#include "osc_map_free.h"

#include <stdlib.h>
#include <string.h>

#include "desk_surface_free.h"

void osc_map_free(struct osc_map *map) {
    desk_surface_free(&map->surface);
    free(map->address);
    free(map->factor_address);
    free(map->function);
    memset(map, 0, sizeof *map);
    map->grand_master = -1;
}
