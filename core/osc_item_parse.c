#include "osc_item_parse.h"

#include <limits.h>
#include <string.h>

#include "osc_json_address.h"
#include "osc_json_bool.h"
#include "osc_json_int.h"
#include "osc_kind_of.h"

const char *osc_item_parse(const cJSON *node, struct desk_surface_item *item, char *address,
                           char *factor_address) {
    memset(item, 0, sizeof *item);
    item->group_id = item->function_id = -1;
    item->dial_ms = item->dial_factor = item->dial_max_ms = -1;
    if (!cJSON_IsObject(node))
        return "an item is not an object";
    if (!cJSON_IsNumber(cJSON_GetObjectItemCaseSensitive(node, "id")) ||
        osc_json_int(node, "id", 0, INT_MAX, &item->id) != 0)
        return "an item has no id";
    const cJSON *kind = cJSON_GetObjectItemCaseSensitive(node, "kind");
    if (!cJSON_IsString(kind) || osc_kind_of(kind->valuestring, &item->kind) != 0)
        return "an item has no known kind";
    if (osc_json_int(node, "group", -1, INT_MAX, &item->group_id) != 0 ||
        osc_json_int(node, "function", -1, INT_MAX, &item->function_id) != 0 ||
        osc_json_bool(node, "disabled", &item->disabled) != 0 ||
        osc_json_bool(node, "on", &item->on) != 0 ||
        osc_json_int(node, "level", 0, 255, &item->level) != 0 ||
        osc_json_int(node, "maxHoldMs", 0, INT_MAX, &item->flash_max_ms) != 0 ||
        osc_json_bool(node, "priority", &item->flash_priority) != 0 ||
        osc_json_int(node, "ms", -1, INT_MAX, &item->dial_ms) != 0 ||
        osc_json_int(node, "factor", -1, 255, &item->dial_factor) != 0 ||
        osc_json_int(node, "maxMs", -1, INT_MAX, &item->dial_max_ms) != 0)
        return "an item has a field out of range";
    if (osc_json_address(node, "address", address) != 0 ||
        osc_json_address(node, "factorAddress", factor_address) != 0)
        return "an item has an address that is not an OSC address";
    if (!address[0] && item->kind != SURFACE_OTHER && item->kind != SURFACE_GROUP)
        return "a control the desk drives has no address";
    if (factor_address[0] && item->kind != SURFACE_DIAL)
        return "only a dial takes a factorAddress";
    return NULL;
}
