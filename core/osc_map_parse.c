#include "osc_map_parse.h"

#include <stdio.h>
#include <string.h>

#include <cjson/cJSON.h>

#include "desk_surface_find.h"
#include "osc_json_bool.h"
#include "osc_json_int.h"
#include "osc_map_free.h"
#include "osc_map_parse_functions.h"
#include "osc_map_parse_items.h"

int osc_map_parse(const char *json, size_t len, struct osc_map *out, char *why, size_t cap) {
    memset(out, 0, sizeof *out);
    out->grand_master = -1;
    const char *wrong = NULL;
    int schema = 0, grand_master = -1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    if (!cJSON_IsObject(root))
        wrong = "not a JSON object";
    else if (osc_json_int(root, "osc", 1, 1, &schema) != 0 || schema != 1)
        wrong = "not an OSC surface file (\"osc\": 1)";
    else if (osc_json_bool(root, "feedback", &out->feedback) != 0 ||
             osc_json_int(root, "grandMaster", -1, 0x7fffffff, &grand_master) != 0)
        wrong = "feedback or grandMaster is malformed";
    else
        wrong = osc_map_parse_items(cJSON_GetObjectItemCaseSensitive(root, "items"), out);
    if (!wrong)
        wrong = osc_map_parse_functions(cJSON_GetObjectItemCaseSensitive(root, "functions"), out);
    const cJSON *version = root ? cJSON_GetObjectItemCaseSensitive(root, "version") : NULL;
    if (!wrong && version && (!cJSON_IsString(version) ||
                              strlen(version->valuestring) >= sizeof out->surface.version))
        wrong = "version is not a short string";
    else if (!wrong && version)
        strcpy(out->surface.version, version->valuestring);
    if (!wrong && grand_master >= 0) {
        const struct desk_surface_item *gm = desk_surface_find(&out->surface, grand_master);
        if (!gm || gm->kind != SURFACE_LEVEL)
            wrong = "grandMaster is not a level item";
        else
            out->grand_master = (int)(gm - out->surface.item);
    }
    cJSON_Delete(root);
    if (!wrong)
        return 0;
    snprintf(why, cap, "%s", wrong);
    osc_map_free(out);
    return -1;
}
