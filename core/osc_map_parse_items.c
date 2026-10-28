#include "osc_map_parse_items.h"

#include <stdlib.h>

#include "osc_factor_presses_parse.h"
#include "osc_item_parse.h"
#include "osc_json_bool.h"

const char *osc_map_parse_items(const cJSON *items, struct osc_map *map) {
    if (!cJSON_IsArray(items))
        return "no items";
    int n = cJSON_GetArraySize(items);
    map->surface.item = calloc((size_t)(n ? n : 1), sizeof *map->surface.item);
    map->address = calloc((size_t)(n ? n : 1), sizeof *map->address);
    map->factor_address = calloc((size_t)(n ? n : 1), sizeof *map->factor_address);
    map->ms_range = calloc((size_t)(n ? n : 1), sizeof *map->ms_range);
    map->reports = calloc((size_t)(n ? n : 1), sizeof *map->reports);
    if (!map->surface.item || !map->address || !map->factor_address || !map->ms_range ||
        !map->reports)
        return "out of memory";
    const cJSON *node;
    cJSON_ArrayForEach(node, items) {
        int i = map->surface.count;
        const char *why = osc_item_parse(node, &map->surface.item[i], map->address[i],
                                         map->factor_address[i], map->ms_range[i]);
        if (why)
            return why;
        map->reports[i] = map->feedback;
        if (osc_json_bool(node, "feedback", &map->reports[i]) != 0 ||
            (map->reports[i] && !map->feedback))
            return "an item's feedback can only turn off its console's";
        if (i > 0 && map->surface.item[i - 1].id >= map->surface.item[i].id)
            return "item ids do not rise strictly";
        map->surface.count++;
        why = osc_factor_presses_parse(node, i, map);
        if (why)
            return why;
    }
    return NULL;
}
