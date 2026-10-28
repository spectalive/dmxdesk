#include "qlc_osc_surface_json.h"

#include <cjson/cJSON.h>

#include "desk_surface_free.h"
#include "qlc_osc_item_json.h"
#include "qlc_surface_of.h"

char *qlc_osc_surface_json(const struct vc_doc *doc, const char *prefix, int grand_master,
                           int feedback) {
    struct desk_surface s;
    if (qlc_surface_of(doc, &s) != 0)
        return NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON *items = root ? cJSON_CreateArray() : NULL;
    int ok = items != NULL;
    if (ok) {
        cJSON_AddNumberToObject(root, "osc", 1);
        cJSON_AddStringToObject(root, "version", s.version);
        cJSON_AddBoolToObject(root, "feedback", feedback);
        if (grand_master >= 0)
            cJSON_AddNumberToObject(root, "grandMaster", grand_master);
        cJSON_AddItemToObject(root, "items", items);
    }
    for (int i = 0; ok && i < s.count; i++) {
        cJSON *item = qlc_osc_item_json(&doc->widget[i], &s.item[i], prefix, feedback);
        ok = item != NULL;
        if (ok)
            cJSON_AddItemToArray(items, item);
    }
    char *text = ok ? cJSON_Print(root) : NULL;
    cJSON_Delete(root);     // items, once created, always hang from root
    desk_surface_free(&s);
    return text;
}
