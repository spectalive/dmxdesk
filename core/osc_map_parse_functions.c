#include "osc_map_parse_functions.h"

#include <stdlib.h>

#include "osc_function_parse.h"

const char *osc_map_parse_functions(const cJSON *functions, struct osc_map *map) {
    if (!functions)
        return NULL;
    if (!cJSON_IsArray(functions))
        return "functions is not a list";
    int n = cJSON_GetArraySize(functions);
    map->function = calloc((size_t)(n ? n : 1), sizeof *map->function);
    if (!map->function)
        return "out of memory";
    const cJSON *node;
    cJSON_ArrayForEach(node, functions) {
        const char *why = osc_function_parse(node, &map->function[map->functions]);
        if (why)
            return why;
        map->functions++;
    }
    return NULL;
}
