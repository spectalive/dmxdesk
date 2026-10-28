#include "osc_factor_presses_parse.h"

#include <stdlib.h>
#include <string.h>

#include "osc_factor_key.h"

const char *osc_factor_presses_parse(const cJSON *node, int item, struct osc_map *map) {
    const cJSON *presses = cJSON_GetObjectItemCaseSensitive(node, "factorAddresses");
    if (!presses)
        return NULL;
    if (map->surface.item[item].kind != SURFACE_DIAL || map->factor_address[item][0])
        return "factorAddresses is for a dial without a factorAddress";
    if (!cJSON_IsObject(presses))
        return "factorAddresses is not an object";
    const cJSON *entry;
    cJSON_ArrayForEach(entry, presses) {
        int factor = osc_factor_key(entry->string);
        if (factor < 0)
            return "a factorAddresses key is not a multiplier 0..10";
        if (!cJSON_IsString(entry) || entry->valuestring[0] != '/' ||
            strlen(entry->valuestring) >= OSC_ADDRESS_MAX)
            return "a factorAddresses value is not an OSC address";
        struct osc_factor_press *grown =
            realloc(map->factor_press, (size_t)(map->factor_presses + 1) * sizeof *grown);
        if (!grown)
            return "out of memory";
        map->factor_press = grown;
        struct osc_factor_press *fp = &map->factor_press[map->factor_presses++];
        fp->item = item;
        fp->factor = factor;
        strcpy(fp->address, entry->valuestring);
    }
    return NULL;
}
