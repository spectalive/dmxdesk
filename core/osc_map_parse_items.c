#include "osc_map_parse_items.h"

#include <stdlib.h>

#include "osc_item_parse.h"

const char *osc_map_parse_items(const cJSON *items, struct osc_map *map) {
    if (!cJSON_IsArray(items))
        return "no items";
    int n = cJSON_GetArraySize(items);
    map->surface.item = calloc((size_t)(n ? n : 1), sizeof *map->surface.item);
    map->address = calloc((size_t)(n ? n : 1), sizeof *map->address);
    map->factor_address = calloc((size_t)(n ? n : 1), sizeof *map->factor_address);
    if (!map->surface.item || !map->address || !map->factor_address)
        return "out of memory";
    const cJSON *node;
    cJSON_ArrayForEach(node, items) {
        int i = map->surface.count;
        const char *why = osc_item_parse(node, &map->surface.item[i], map->address[i],
                                         map->factor_address[i]);
        if (why)
            return why;
        if (i > 0 && map->surface.item[i - 1].id >= map->surface.item[i].id)
            return "item ids do not rise strictly";
        map->surface.count++;
    }
    return NULL;
}
