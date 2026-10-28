#include "osc_json_bool.h"

int osc_json_bool(const cJSON *node, const char *name, int *out) {
    const cJSON *field = cJSON_GetObjectItemCaseSensitive(node, name);
    if (!field)
        return 0;
    if (!cJSON_IsBool(field))
        return -1;
    *out = cJSON_IsTrue(field);
    return 0;
}
