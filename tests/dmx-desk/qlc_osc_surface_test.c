// SOURCES: showmap.c showmap_release_to_parse.c showmap_release_to_prune.c showmap_validate.c showmap_check_priority.c desk_model.c desk_placement_hit.c desk_placement_target.c desk_master_level_at.c desk_layout_resolve.c desk_show_layout.c vcjson.c desk_family_icon.c icon_glyph.c desk_show_slot.c desk_show_place.c desk_show_panel.c desk_show_section.c desk_show_accent_tile.c desk_show_live_page.c desk_show_is_rig.c desk_pick_numbered.c desk_caption_number.c desk_trajectory_of.c desk_show_holds.c desk_show_haze.c desk_show_rig.c desk_damage_running.c desk_panel_holds.c desk_surface_find.c desk_surface_free.c desk_surface_is_button.c qlc_surface_kind.c qlc_surface_of.c osc_map_parse.c osc_map_parse_items.c osc_map_parse_functions.c osc_item_parse.c osc_function_parse.c osc_factor_presses_parse.c osc_factor_key.c osc_kind_of.c osc_json_int.c osc_json_bool.c osc_json_address.c osc_json_range.c osc_map_free.c osc_kind_word.c qlc_osc_surface_json.c qlc_osc_item_json.c qlc_osc_channel.c qlc_osc_factor_input.c qlc_osc_channels_write.c
// QLC+'s /vc.json as an OSC surface file for QLC+'s own OSC input
// (2026-10-28, after the trial that drove a headless QLC+ 5.2.2 over OSC).
// The file reads back as the same surface the QLC+ adapter builds, but with
// what that trial found: hidden-page widgets disabled, dials taking their
// time as 0..1 of their range and their multiplier as one button per factor,
// dials and faders marked as not reported. The channels are the ones QLC+
// accepted in that trial.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desk_model.h"
#include "desk_surface_find.h"
#include "desk_surface_free.h"
#include "osc_kind_of.h"
#include "osc_kind_word.h"
#include "osc_map_free.h"
#include "osc_map_parse.h"
#include "qlc_osc_channel.h"
#include "qlc_osc_channels_write.h"
#include "qlc_osc_factor_input.h"
#include "qlc_osc_surface_json.h"
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

static struct osc_map converted(const struct vc_doc *doc, const char *prefix, int gm, int feedback) {
    char *text = qlc_osc_surface_json(doc, prefix, gm, feedback);
    assert(text);
    struct osc_map map;
    char why[128];
    assert(osc_map_parse(text, strlen(text), &map, why, sizeof why) == 0);
    free(text);
    return map;
}

static struct desk_model model_q, model_o;

int main(void) {
    // The channels QLC+'s OSC plugin derived, as the trial's workspace mapped
    // them and QLC+ answered to them.
    assert(qlc_osc_channel("/vibra/33") == 32623);
    assert(qlc_osc_channel("/vibra/33/x2") == 40739);
    assert(qlc_osc_channel("/vibra/33/x6") == 55559);
    assert(qlc_osc_channel("/vibra/33/x10") == 63637);
    assert(qlc_osc_factor_input(2) == 6 && qlc_osc_factor_input(5) == 9);
    assert(qlc_osc_factor_input(6) == 4 && qlc_osc_factor_input(7) == 10);
    assert(qlc_osc_factor_input(10) == 13);
    assert(qlc_osc_factor_input(0) == -1 && qlc_osc_factor_input(1) == -1);
    assert(qlc_osc_factor_input(11) == -1 && qlc_osc_factor_input(-1) == -1);
    for (int k = SURFACE_OTHER; k <= SURFACE_DIAL; k++) {
        enum desk_surface_kind back;
        assert(osc_kind_of(osc_kind_word((enum desk_surface_kind)k), &back) == 0 && (int)back == k);
    }

    size_t len;
    char *json = slurp("tests/dmx-desk/fixtures/vc-vibra.json", &len);
    struct vc_doc doc;
    assert(vc_parse(json, len, &doc) == 0);
    free(json);
    struct desk_surface q;
    assert(qlc_surface_of(&doc, &q) == 0);
    struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);

    struct osc_map o = converted(&doc, "/vibra", map.grand_master_widget, 1);
    assert(o.feedback && o.surface.count == q.count && !strcmp(o.surface.version, q.version));
    assert(o.surface.item[o.grand_master].id == map.grand_master_widget);
    int hidden = 0, dials = 0;
    for (int i = 0; i < q.count; i++) {
        const struct desk_surface_item *a = &q.item[i], *b = &o.surface.item[i];
        struct desk_surface_item same = *b;
        same.disabled = a->disabled;
        assert(!memcmp(&same, a, sizeof same));
        assert(b->disabled == (a->disabled || !doc.widget[i].visible));
        hidden += !doc.widget[i].visible;
        char want[64] = "";
        if (a->kind != SURFACE_OTHER && a->kind != SURFACE_GROUP)
            snprintf(want, sizeof want, "/vibra/%d", a->id);
        assert(!strcmp(o.address[i], want) && !o.factor_address[i][0]);
        assert(o.reports[i] == (a->kind != SURFACE_DIAL && a->kind != SURFACE_LEVEL));
        if (a->kind == SURFACE_DIAL) {
            dials++;
            assert(o.ms_range[i][0] == doc.widget[i].speed_min_ms &&
                   o.ms_range[i][1] == doc.widget[i].speed_max_ms && o.ms_range[i][1] > 0);
        } else {
            assert(o.ms_range[i][0] == -1);
        }
    }
    assert(hidden > 0 && dials == 2 && o.factor_presses == 9 * dials);
    for (int i = 0; i < o.factor_presses; i++) {
        char want[64];
        snprintf(want, sizeof want, "/vibra/%d/x%d", o.surface.item[o.factor_press[i].item].id,
                 o.factor_press[i].factor);
        assert(!strcmp(o.factor_press[i].address, want));
        assert(qlc_osc_factor_input(o.factor_press[i].factor) >= 0);
    }

    // The desk built against it: what is enabled over OSC is enabled over
    // QLC+'s own web access too, and each control on a hidden page is not.
    int enabled_q = showmap_build(&model_q, &map, &q);
    int enabled_o = showmap_build(&model_o, &map, &o.surface);
    assert(enabled_o > 0 && enabled_o < enabled_q && model_o.count == model_q.count);
    int lost = 0;
    for (int i = 0; i < model_o.count; i++) {
        const struct desk_control *c = &model_o.control[i];
        assert(!c->enabled || model_q.control[i].enabled);
        const struct desk_surface_item *it = desk_surface_find(&o.surface, c->widget_id);
        if (it && it->disabled && model_q.control[i].enabled) {
            assert(!c->enabled);
            lost++;
        }
    }
    assert(lost > 0);
    // Vibra's grand master is on a hidden page: its fader is not offered.
    int master = model_o.count;
    for (int i = 0; i < model_o.count; i++)
        if (model_o.control[i].kind == DESK_MASTER)
            master = i;
    assert(master < model_o.count && model_q.control[master].enabled);
    assert(!model_o.control[master].enabled &&
           !strcmp(model_o.control[master].reason, "disabled on the master"));

    // The table of what QLC+ must listen to: every address, then every
    // multiplier button, each on the channel its path gives.
    FILE *t = tmpfile();
    assert(t);
    int addressed = 0;
    for (int i = 0; i < o.surface.count; i++)
        addressed += o.address[i][0] != '\0';
    assert(qlc_osc_channels_write(&o, t) == addressed + o.factor_presses);
    rewind(t);
    char line[160];
    int saw_wheel = 0, saw_one = 0;
    while (fgets(line, sizeof line, t)) {
        saw_wheel |= !strcmp(line, "33\t0\t32623\t/vibra/33\n");
        saw_one |= !strcmp(line, "33\t4\t55559\t/vibra/33/x6\n");
    }
    assert(saw_wheel && saw_one);
    fclose(t);
    osc_map_free(&o);

    // A workspace with no OSC feedback patch: nothing is reported back.
    o = converted(&doc, "/desk", -1, 0);
    assert(!o.feedback && o.grand_master == -1);
    for (int i = 0; i < o.surface.count; i++)
        assert(!o.reports[i] && (!o.address[i][0] || !strncmp(o.address[i], "/desk/", 6)));
    osc_map_free(&o);

    // A dial whose document gives no range keeps its time in ms.
    struct vc_widget dial;
    memset(&dial, 0, sizeof dial);
    dial.id = 5;
    dial.type_id = VC_SPEED_DIAL;
    dial.visible = 1;
    dial.speed_ms = 500;
    dial.speed_factor = 6;
    dial.speed_min_ms = dial.speed_max_ms = -1;
    struct vc_doc single = { "5.2.2", 1, 1, &dial };
    o = converted(&single, "/d", -1, 1);
    assert(o.ms_range[0][0] == -1 && o.factor_presses == 9 && !strcmp(o.address[0], "/d/5"));
    assert(!o.surface.item[0].disabled);
    osc_map_free(&o);

    desk_surface_free(&q);
    vc_free(&doc);
    return 0;
}
