#include "showmap_release_to_parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDGET_ID_MAX 0x7FFFFFF

void showmap_release_to_parse(const cJSON *item, struct map_control *c) {
    c->releases = 0;
    const cJSON *field = cJSON_GetObjectItemCaseSensitive(item, "releaseTo");
    if (!field || cJSON_IsNull(field))
        return;
    if (!cJSON_IsObject(field)) {
        fprintf(stderr, "map: control %s: releaseTo is not an object, ignored\n", c->key);
        return;
    }
    const cJSON *entry = NULL;
    cJSON_ArrayForEach(entry, field) {
        const char *key = entry->string ? entry->string : "";
        size_t digits = strspn(key, "0123456789");
        long state = digits > 0 && digits <= 9 && key[digits] == '\0' ? strtol(key, NULL, 10) : -1;
        double hook = cJSON_IsNumber(entry) ? cJSON_GetNumberValue(entry) : -1;
        if (state < 0 || state > WIDGET_ID_MAX || hook != (double)(int)hook || hook < 0 ||
            hook > WIDGET_ID_MAX) {
            fprintf(stderr, "map: control %s: releaseTo entry %.20s is not a widget pair, ignored\n",
                    c->key, key);
            continue;
        }
        if (c->releases >= MAP_MAX_RELEASES) {
            fprintf(stderr, "map: control %s: more than %d releaseTo entries, the rest ignored\n",
                    c->key, MAP_MAX_RELEASES);
            return;
        }
        c->release_to[c->releases].state_widget = (int)state;
        c->release_to[c->releases].hook_widget = (int)hook;
        c->releases++;
    }
}
