#include "osc_json_range.h"

#include <limits.h>

int osc_json_range(const cJSON *node, const char *name, int out[2]) {
    out[0] = out[1] = -1;
    const cJSON *field = cJSON_GetObjectItemCaseSensitive(node, name);
    if (!field)
        return 0;
    if (!cJSON_IsArray(field) || cJSON_GetArraySize(field) != 2)
        return -1;
    double v[2];
    for (int i = 0; i < 2; i++) {
        const cJSON *n = cJSON_GetArrayItem(field, i);
        if (!cJSON_IsNumber(n) || n->valuedouble < 0 || n->valuedouble > INT_MAX ||
            n->valuedouble != (double)(int)n->valuedouble)
            return -1;
        v[i] = n->valuedouble;
    }
    if (v[0] >= v[1])
        return -1;
    out[0] = (int)v[0];
    out[1] = (int)v[1];
    return 0;
}
