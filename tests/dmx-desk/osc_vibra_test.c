// SOURCES: showmap.c showmap_release_to_parse.c showmap_release_to_prune.c showmap_validate.c showmap_check_priority.c desk_model.c desk_placement_hit.c desk_placement_target.c desk_master_level_at.c desk_layout_resolve.c desk_show_layout.c vcjson.c desk_family_icon.c icon_glyph.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_show_holds.c desk_show_haze.c desk_show_rig.c desk_damage_running.c desk_panel_holds.c desk_surface_find.c desk_surface_free.c desk_surface_is_button.c qlc_surface_kind.c qlc_surface_of.c desk_speed.c desk_tap.c speed_factor.c desk_speed_target_rect.c osc_map_parse.c osc_map_parse_items.c osc_map_parse_functions.c osc_item_parse.c osc_function_parse.c osc_kind_of.c osc_json_int.c osc_json_bool.c osc_json_address.c osc_map_free.c osc_command_messages.c osc_msg_make.c osc_json_range.c osc_factor_presses_parse.c osc_factor_key.c osc_dial_fraction.c osc_factor_press_find.c
// The Vibra show over OSC (2026-10-28): the console QLC+ serves, written
// out as an OSC surface file, comes back as the very same surface, and the
// desk builds the very same model and speed dials from it. So a console
// that is not QLC+ needs nothing but a surface file to carry this show, and
// each control the map names gets an address of its own.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>

#include "desk_model.h"
#include "desk_speed.h"
#include "desk_surface_find.h"
#include "desk_surface_free.h"
#include "osc_command_messages.h"
#include "osc_map_free.h"
#include "osc_map_parse.h"
#include "qlc_surface_of.h"
#include "showmap.h"
#include "showmap_validate.h"
#include "vcjson.h"

static char *slurp(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    assert(f);
    assert(fseek(f, 0, SEEK_END) == 0);
    long size = ftell(f);
    assert(size > 0 && fseek(f, 0, SEEK_SET) == 0);
    char *buf = malloc((size_t)size + 1);
    assert(buf && fread(buf, 1, (size_t)size, f) == (size_t)size);
    buf[size] = '\0';
    fclose(f);
    *len = (size_t)size;
    return buf;
}

static const char *kind_word(enum desk_surface_kind k) {
    static const char *words[] = { "other", "group", "toggle", "flash", "stop-all", "button",
                                   "level", "dial" };
    return words[k];
}

// The surface as a console's OSC file would declare it: every field, and an
// address per control at /vibra/<id>.
static char *as_osc_file(const struct desk_surface *s, int grand_master) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "osc", 1);
    cJSON_AddStringToObject(root, "version", s->version);
    cJSON_AddNumberToObject(root, "grandMaster", grand_master);
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    for (int i = 0; i < s->count; i++) {
        const struct desk_surface_item *it = &s->item[i];
        cJSON *o = cJSON_CreateObject();
        char address[64];
        cJSON_AddNumberToObject(o, "id", it->id);
        cJSON_AddStringToObject(o, "kind", kind_word(it->kind));
        cJSON_AddNumberToObject(o, "group", it->group_id);
        cJSON_AddNumberToObject(o, "function", it->function_id);
        cJSON_AddBoolToObject(o, "disabled", it->disabled);
        cJSON_AddBoolToObject(o, "on", it->on);
        cJSON_AddNumberToObject(o, "level", it->level);
        cJSON_AddNumberToObject(o, "maxHoldMs", it->flash_max_ms);
        cJSON_AddBoolToObject(o, "priority", it->flash_priority);
        cJSON_AddNumberToObject(o, "ms", it->dial_ms);
        cJSON_AddNumberToObject(o, "factor", it->dial_factor);
        cJSON_AddNumberToObject(o, "maxMs", it->dial_max_ms);
        if (it->kind != SURFACE_OTHER && it->kind != SURFACE_GROUP) {
            snprintf(address, sizeof address, "/vibra/%d", it->id);
            cJSON_AddStringToObject(o, "address", address);
        }
        if (it->kind == SURFACE_DIAL) {
            snprintf(address, sizeof address, "/vibra/%d/factor", it->id);
            cJSON_AddStringToObject(o, "factorAddress", address);
        }
        cJSON_AddItemToArray(items, o);
    }
    char *text = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    assert(text);
    return text;
}

static struct desk_model model_q, model_o;
static struct desk_speed speed_q, speed_o;

int main(void) {
    size_t len;
    char *json = slurp("tests/dmx-desk/fixtures/vc-vibra.json", &len);
    struct vc_doc doc;
    assert(vc_parse(json, len, &doc) == 0);
    free(json);
    struct desk_surface q;
    assert(qlc_surface_of(&doc, &q) == 0);
    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    assert(map.grand_master_widget >= 0);

    char *file = as_osc_file(&q, map.grand_master_widget);
    struct osc_map o;
    char why[128];
    assert(osc_map_parse(file, strlen(file), &o, why, sizeof why) == 0);
    free(file);

    // The same surface, item for item.
    assert(o.surface.count == q.count && !strcmp(o.surface.version, q.version));
    for (int i = 0; i < q.count; i++)
        assert(!memcmp(&o.surface.item[i], &q.item[i], sizeof q.item[i]));
    assert(o.surface.item[o.grand_master].id == map.grand_master_widget);

    // The same model: every control enabled or disabled alike, with the
    // same reason, and the same speed dials.
    int enabled_q = showmap_build(&model_q, &map, &q);
    int enabled_o = showmap_build(&model_o, &map, &o.surface);
    assert(enabled_q == enabled_o && enabled_q > 0 && model_q.count == model_o.count);
    for (int i = 0; i < model_q.count; i++) {
        assert(model_q.control[i].enabled == model_o.control[i].enabled);
        assert(!strcmp(model_q.control[i].reason, model_o.control[i].reason));
    }
    assert(!memcmp(&model_q, &model_o, sizeof model_q));
    desk_speed_init(&speed_q, &map);
    desk_speed_init(&speed_o, &map);
    desk_speed_validate(&speed_q, &q);
    desk_speed_validate(&speed_o, &o.surface);
    assert(speed_q.dials > 0 && !memcmp(&speed_q, &speed_o, sizeof speed_q));

    // Every enabled cue of the map has an address, and a press goes there.
    int pressed = 0;
    for (int i = 0; i < model_o.count; i++) {
        const struct desk_control *c = &model_o.control[i];
        if (!c->enabled || c->kind != DESK_CUE)
            continue;
        struct desk_command cmd = { DESK_CMD_TOGGLE, c->widget_id, 0 };
        struct osc_msg out[2];
        char want[64];
        snprintf(want, sizeof want, "/vibra/%d", c->widget_id);
        assert(osc_command_messages(&o, &cmd, out) == 2 && !strcmp(out[0].address, want));
        pressed++;
    }
    assert(pressed > 10);
    struct desk_command gm = { DESK_CMD_GRAND_MASTER, 0, 128 };
    struct osc_msg out[2];
    assert(osc_command_messages(&o, &gm, out) == 1);

    osc_map_free(&o);
    desk_surface_free(&q);
    vc_free(&doc);
    return 0;
}
