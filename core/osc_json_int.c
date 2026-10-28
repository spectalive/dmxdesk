#include "osc_json_int.h"

int osc_json_int(const cJSON *node, const char *name, int min, int max, int *out) {
    const cJSON *field = cJSON_GetObjectItemCaseSensitive(node, name);
    if (!field)
        return 0;
    if (!cJSON_IsNumber(field) || field->valuedouble < min || field->valuedouble > max)
        return -1;
    int v = (int)field->valuedouble;
    if ((double)v != field->valuedouble)
        return -1;
    *out = v;
    return 0;
}
