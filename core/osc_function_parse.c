#include "osc_function_parse.h"

#include <limits.h>
#include <string.h>

#include "osc_json_address.h"
#include "osc_json_int.h"

const char *osc_function_parse(const cJSON *node, struct osc_function *out) {
    memset(out, 0, sizeof *out);
    if (!cJSON_IsObject(node) || !cJSON_IsNumber(cJSON_GetObjectItemCaseSensitive(node, "id")) ||
        osc_json_int(node, "id", 0, INT_MAX, &out->id) != 0)
        return "a function has no id";
    if (osc_json_address(node, "address", out->address) != 0 || !out->address[0])
        return "a function has no OSC address";
    return NULL;
}
