#include "qlc_osc_item_json.h"

#include <stdio.h>

#include "osc_kind_word.h"
#include "osc_msg.h"
#include "qlc_osc_factor_input.h"

cJSON *qlc_osc_item_json(const struct vc_widget *w, const struct desk_surface_item *it,
                         const char *prefix, int feedback) {
    cJSON *o = cJSON_CreateObject();
    if (!o)
        return NULL;
    char address[OSC_ADDRESS_MAX];
    cJSON_AddNumberToObject(o, "id", it->id);
    cJSON_AddStringToObject(o, "kind", osc_kind_word(it->kind));
    cJSON_AddNumberToObject(o, "group", it->group_id);
    cJSON_AddNumberToObject(o, "function", it->function_id);
    cJSON_AddBoolToObject(o, "disabled", it->disabled || !w->visible);
    cJSON_AddBoolToObject(o, "on", it->on);
    cJSON_AddNumberToObject(o, "level", it->level);
    cJSON_AddNumberToObject(o, "maxHoldMs", it->flash_max_ms);
    cJSON_AddBoolToObject(o, "priority", it->flash_priority);
    cJSON_AddNumberToObject(o, "ms", it->dial_ms);
    cJSON_AddNumberToObject(o, "factor", it->dial_factor);
    cJSON_AddNumberToObject(o, "maxMs", it->dial_max_ms);
    if (it->kind != SURFACE_OTHER && it->kind != SURFACE_GROUP) {
        snprintf(address, sizeof address, "%s/%d", prefix, it->id);
        cJSON_AddStringToObject(o, "address", address);
    }
    if (feedback && (it->kind == SURFACE_DIAL || it->kind == SURFACE_LEVEL))
        cJSON_AddBoolToObject(o, "feedback", 0);
    if (it->kind != SURFACE_DIAL)
        return o;
    if (w->speed_min_ms >= 0 && w->speed_max_ms > w->speed_min_ms) {
        cJSON *range = cJSON_AddArrayToObject(o, "msRange");
        cJSON_AddItemToArray(range, cJSON_CreateNumber(w->speed_min_ms));
        cJSON_AddItemToArray(range, cJSON_CreateNumber(w->speed_max_ms));
    }
    cJSON *presses = cJSON_AddObjectToObject(o, "factorAddresses");
    for (int f = 0; f <= 10; f++) {
        if (qlc_osc_factor_input(f) < 0)
            continue;
        char key[4];
        snprintf(key, sizeof key, "%d", f);
        snprintf(address, sizeof address, "%s/%d/x%d", prefix, it->id, f);
        cJSON_AddStringToObject(presses, key, address);
    }
    return o;
}
