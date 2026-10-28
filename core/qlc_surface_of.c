#include "qlc_surface_of.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qlc_surface_kind.h"

int qlc_surface_of(const struct vc_doc *doc, struct desk_surface *out) {
    memset(out, 0, sizeof *out);
    if (doc->count > 0) {
        out->item = calloc((size_t)doc->count, sizeof *out->item);
        if (!out->item)
            return -1;
    }
    snprintf(out->version, sizeof out->version, "%s", doc->app_version);
    for (int i = 0; i < doc->count; i++) {
        const struct vc_widget *w = &doc->widget[i];
        struct desk_surface_item *it = &out->item[i];
        it->id = w->id;
        it->kind = qlc_surface_kind(w);
        it->group_id = w->parent_id;
        it->function_id = w->function_id;
        it->disabled = w->disabled;
        it->on = w->state;
        it->level = w->value;
        it->flash_max_ms = w->flash_max_hold_ms;
        it->flash_priority = w->flash_override && w->flash_force_ltp;
        it->dial_ms = w->speed_ms;
        it->dial_factor = w->speed_factor;
        it->dial_max_ms = w->speed_max_ms;
    }
    out->count = doc->count;
    return 0;
}
