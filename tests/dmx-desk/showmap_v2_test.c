// SOURCES: showmap.c showmap_release_to_parse.c showmap_release_to_prune.c showmap_validate.c desk_model.c desk_master_level_at.c desk_layout_resolve.c desk_show_layout.c vcjson.c
// The generated map (schema 2) parsed with its bounds, and built against the
// console the master really serves: the room's states pressable, the held
// hits carried disabled, the panic button live, and a console that is not
// the show disabling everything with one reason.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cjson/cJSON.h>

#include "desk_layout.h"
#include "desk_layout_resolve.h"
#include "desk_model.h"
#include "showmap.h"
#include "showmap_validate.h"
#include "vcjson.h"

static char *slurp(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    assert(f);
    assert(fseek(f, 0, SEEK_END) == 0);
    long size = ftell(f);
    assert(size > 0);
    assert(fseek(f, 0, SEEK_SET) == 0);
    char *buf = malloc((size_t)size + 1);
    assert(buf);
    assert(fread(buf, 1, (size_t)size, f) == (size_t)size);
    fclose(f);
    buf[size] = '\0';
    *len = (size_t)size;
    return buf;
}

// The map text with one substring replaced, for the refusals.
static char *edited(const char *text, const char *from, const char *to) {
    const char *at = strstr(text, from);
    assert(at);
    size_t before = (size_t)(at - text), flen = strlen(from), tlen = strlen(to);
    char *out = malloc(strlen(text) - flen + tlen + 1);
    assert(out);
    memcpy(out, text, before);
    memcpy(out + before, to, tlen);
    strcpy(out + before + tlen, at + flen);
    return out;
}

// An integer from the map's own JSON, read with cJSON rather than the parser
// under test: the ids move with every regenerated show, and the test should
// check that the parser reports what the file says, not one show's numbers.
static int map_int(const cJSON *root, const char *a, const char *b, const char *c) {
    const cJSON *n = cJSON_GetObjectItemCaseSensitive(root, a);
    if (b)
        n = cJSON_GetObjectItemCaseSensitive(n, b);
    if (c)
        n = cJSON_GetObjectItemCaseSensitive(n, c);
    assert(cJSON_IsNumber(n));
    return n->valueint;
}

// A dial's member count and its first member's function and raw duration.
static const cJSON *dial_members(const cJSON *root, const char *key) {
    const cJSON *m = cJSON_GetObjectItemCaseSensitive(
        cJSON_GetObjectItemCaseSensitive(cJSON_GetObjectItemCaseSensitive(root, "dials"), key), "members");
    assert(cJSON_IsArray(m) && cJSON_GetArraySize(m) > 0);
    return m;
}

static const struct desk_control *by_label(const struct desk_model *m, const char *label) {
    for (int i = 0; i < m->count; i++)
        if (strcmp(m->control[i].label, label) == 0)
            return &m->control[i];
    return NULL;
}

static const struct desk_control *by_kind(const struct desk_model *m, enum desk_kind kind) {
    for (int i = 0; i < m->count; i++)
        if (m->control[i].kind == kind)
            return &m->control[i];
    return NULL;
}

int main(void) {
    size_t map_len, vc_len;
    char *map_text = slurp("show/vibra.desk.json", &map_len);
    char *vc_text = slurp("tests/dmx-desk/fixtures/vc-vibra.json", &vc_len);

    // The map, as generated.
    struct show_map map;
    assert(showmap_parse(map_text, map_len, &map) == 0);
    cJSON *json = cJSON_Parse(map_text);
    assert(json);
    assert(map.schema == 2 && strcmp(map.qlc_version, "5.2.2") == 0);
    assert(strcmp(map.key, "vibra") == 0);
    assert(map.stop_all_widget == map_int(json, "stopAll", "widget", NULL));
    assert(map.stop_all_fade_ms == map_int(json, "stopAll", "fadeOutMs", NULL));
    assert(map.grand_master_widget == map_int(json, "grandMaster", "widget", NULL));
    assert(map.pages == 7 && strcmp(map.page[0].key, "live") == 0);
    const struct map_section *state = &map.page[0].section[0];
    assert(strcmp(state->key, "state") == 0 && state->count == 7 && state->solo_id == 3);
    assert(state->first == 0);
    const struct map_control *auto_ctl = &map.control[state->first];
    assert(strcmp(auto_ctl->caption, "AUTO") == 0 && auto_ctl->widget_id == map_int(json, "controls", "auto", "widget") &&
           auto_ctl->function_id == map_int(json, "controls", "auto", "function") && auto_ctl->role == MAP_ROLE_STATE &&
           auto_ctl->solo_id == 3 && auto_ctl->page == 0 && auto_ctl->section == 0);
    assert(strcmp(auto_ctl->detail, "el show se lleva solo") == 0);
    const struct map_section *accents = NULL;
    for (int i = 0; i < map.page[0].sections; i++)
        if (strcmp(map.page[0].section[i].key, "accents") == 0)
            accents = &map.page[0].section[i];
    assert(accents && accents->count == 7);
    assert(accents == &map.page[0].section[map.page[0].sections - 1]);
    // The hits are bursts now: the master runs each for its own length and
    // ends it itself, so the tablet may fire them.
    int bursts = 0;
    for (int i = 0; i < accents->count; i++) {
        const struct map_control *c = &map.control[accents->first + i];
        if (c->role == MAP_ROLE_BURST) {
            bursts++;
            assert(!c->held && c->enabled && c->burst_ms > 0 && c->source[0]);
        }
    }
    assert(bursts == 7);
    // A colour pick carries the colour its scenes write.
    int red_found = 0;
    for (int i = 0; i < map.count; i++) {
        if (strcmp(map.control[i].caption, "Rig Rojo") == 0) {
            assert(map.control[i].swatches >= 1 && map.control[i].swatch[0] == 0xff0000u);
            assert(map.control[i].role == MAP_ROLE_PICK);
            red_found = 1;
        }
    }
    assert(red_found);
    // Every control is parsed, each carrying an icon.
    assert(strstr(map_text, "\"icon\":"));
    assert(map.count == cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(json, "controls")));
    // The two dials and the first member of each, as the file has them.
    // Check exact parser output here; behavioural tests resolve dial keys.
    assert(map.dials == 2);
    const char *dial_key[2] = { "tempo-show", "vel-movimiento" };
    for (int d = 0; d < 2; d++) {
        const cJSON *members = dial_members(json, dial_key[d]);
        const cJSON *first = cJSON_GetArrayItem(members, 0);
        assert(strcmp(map.dial[d].key, dial_key[d]) == 0);
        assert(map.dial[d].widget_id == map_int(json, "dials", dial_key[d], "widget"));
        assert(map.dial[d].time_ms == map_int(json, "dials", dial_key[d], "timeMs"));
        assert(map.dial[d].members == cJSON_GetArraySize(members));
        assert(map.dial[d].member[0].function_id == map_int(first, "function", NULL, NULL));
        assert(map.dial[d].member[0].duration == map_int(first, "duration", "raw", NULL));
        assert(map.dial[d].member[0].fade_in == map_int(first, "fadeIn", "raw", NULL));
    }

    // Refusals: a widget listed twice, a section naming a missing control, a
    // swatch that is not a colour, a wrong schema.
    struct show_map bad;
    char *dup = edited(map_text, "\"widget\": 5,", "\"widget\": 4,");
    assert(showmap_parse(dup, strlen(dup), &bad) == -1);
    free(dup);
    char *missing = edited(map_text, "\"auto\",", "\"auto\", \"nobody\",");
    assert(showmap_parse(missing, strlen(missing), &bad) == -1);
    free(missing);
    char *swatch = edited(map_text, "\"#ff0000\"", "\"red\"");
    assert(showmap_parse(swatch, strlen(swatch), &bad) == -1);
    free(swatch);
    char *schema = edited(map_text, "\"schema\": 2", "\"schema\": 1");
    assert(showmap_parse(schema, strlen(schema), &bad) == -1);
    free(schema);

    // Built against the real console: seven states, the master, the panic
    // button; the held hits disabled with the map's reason.
    struct vc_doc console;
    assert(vc_parse(vc_text, vc_len, &console) == 0);
    struct desk_model model;
    int enabled = showmap_build(&model, &map, &console);
    assert(model.count == map.count + 4);
    struct desk_layout layout;
    assert(desk_layout_resolve(&map, map.count, map.count + 1, map.count + 2, map.count + 3, &layout) == 0);
    desk_set_layout(&model, &layout);
    const struct desk_control *a = by_label(&model, "AUTO");
    assert(a && a->enabled && a->widget_id == auto_ctl->widget_id && a->function_id == auto_ctl->function_id);
    int auto_ix = (int)(a - model.control);
    const struct desk_placement *ap = desk_placement_of(&model, auto_ix);
    assert(ap && ap->w == 111 && ap->x == DESK_CONTENT_X && ap->y == 76);
    // Every hit is a burst the master bounds, fog included.
    const struct desk_control *flash = by_label(&model, "FLASH");
    assert(flash && flash->enabled && flash->kind == DESK_BURST && flash->burst_ms == 8000);
    const struct desk_control *fog = by_label(&model, "HUMO YA");
    assert(fog && fog->enabled && fog->kind == DESK_BURST && fog->burst_ms == 3000);
    const struct desk_control *rojo = by_label(&model, "Rig Rojo");
    assert(rojo && rojo->enabled && rojo->swatches == 1);
    const struct desk_placement *rmini = desk_placement_of(&model, (int)(rojo - model.control));
    assert(rmini && rmini->tile == TILE_MINI);    // on SHOW as a rig colour
    const struct desk_control *stop = by_kind(&model, DESK_STOP_ALL);
    assert(stop && stop->enabled && stop->widget_id == map.stop_all_widget);
    const struct desk_placement *sp = desk_placement_of(&model, (int)(stop - model.control));
    assert(sp && sp->y == DESK_PANIC_Y);
    assert(strcmp(stop->detail, "1.0 s fade") == 0);
    const struct desk_control *master = by_kind(&model, DESK_MASTER);
    // Known from the snapshot: the console carries the slider's level.
    assert(master && master->enabled && master->state == DESK_ON && master->level == 255);
    // Nothing is held any more: every hit is a burst, so the whole map is
    // enabled, plus the master, the stop and SHOW's own two.
    {
        int bursts_on = 0, on = 0;
        for (int i = 0; i < model.count; i++) {
            on += model.control[i].enabled;
            bursts_on += model.control[i].kind == DESK_BURST && model.control[i].enabled;
        }
        assert(bursts_on == 17);           // seven on LIVE, ten colour hits
        assert(enabled == on);
        assert(enabled == map.count + 4);  // every control, the master, the stop, OFF and the tempo
    }

    // The panic button is a completed tap when READY, and nothing otherwise.
    int sx = sp->x + 10, sy = sp->y + 10;
    assert(desk_touch_down(&model, 0, sx, sy, NULL).kind == DESK_ACT_NONE);
    assert(desk_touch_up(&model, 0, sx, sy).kind == DESK_ACT_NONE);
    desk_set_link(&model, DESK_LINK_READY);
    assert(desk_touch_down(&model, 0, sx, sy, NULL).kind == DESK_ACT_NONE);
    struct desk_action panic = desk_touch_up(&model, 0, sx, sy);
    assert(panic.kind == DESK_ACT_STOP_ALL && panic.widget_id == map.stop_all_widget);
    // Nothing lives at the rail or the bar.
    assert(desk_touch_down(&model, 0, 0, 0, NULL).kind == DESK_ACT_NONE);
    // On the COLOR page the compact AUTO fires the same widget on every bank,
    // and Rig Rojo has a place on the bank the resolver gave it.
    int rojo_bank = -1;
    for (int i = 0; i < layout.placements; i++)
        if (layout.placement[i].control == (int)(rojo - model.control) && layout.placement[i].page == 1)
            rojo_bank = layout.placement[i].bank;
    assert(rojo_bank >= 0);
    desk_set_view(&model, 0, 0);
    const struct desk_placement *cap = desk_placement_of(&model, auto_ix);
    assert(cap && cap->tile == TILE_STATE);
    assert(desk_touch_down(&model, 0, cap->x + 5, cap->y + 5, NULL).kind == DESK_ACT_NONE);
    struct desk_action fired = desk_touch_up(&model, 0, cap->x + 5, cap->y + 5);
    assert(fired.kind == DESK_ACT_TOGGLE && fired.widget_id == auto_ctl->widget_id);
    desk_set_view(&model, 1, rojo_bank);
    const struct desk_placement *rp = desk_placement_of(&model, (int)(rojo - model.control));
    assert(rp && rp->tile == TILE_SWATCH);
    // A view change mid-capture cancels the gesture.
    assert(desk_touch_down(&model, 0, rp->x + 5, rp->y + 5, NULL).kind == DESK_ACT_NONE);
    desk_set_view(&model, 0, 0);
    assert(desk_touch_up(&model, 0, rp->x + 5, rp->y + 5).kind == DESK_ACT_NONE);

    // A widget that drives another function than the map says is dead.
    struct vc_doc changed;
    assert(vc_parse(vc_text, vc_len, &changed) == 0);
    // The new snapshot also lists function metadata before its widgets.
    // Mutate the intended widget, not the first textual functionId match.
    for (int i = 0; i < changed.count; i++)
        if (changed.widget[i].id == auto_ctl->widget_id)
            changed.widget[i].function_id = auto_ctl->function_id + 1;
    showmap_build(&model, &map, &changed);
    a = by_label(&model, "AUTO");
    assert(a && !a->enabled && strcmp(a->reason, "drives another cue") == 0);
    vc_free(&changed);

    // Another QLC+ line is another show: everything off, one reason.
    char *newer = edited(vc_text, "\"version\":\"5.2.2\"", "\"version\":\"5.3.0\"");
    assert(vc_parse(newer, strlen(newer), &changed) == 0);
    assert(showmap_build(&model, &map, &changed) == 0);
    for (int i = 0; i < model.count; i++)
        assert(!model.control[i].enabled && strcmp(model.control[i].reason, "show mismatch") == 0);
    vc_free(&changed);
    free(newer);

    vc_free(&console);
    cJSON_Delete(json);
    free(map_text);
    free(vc_text);
    printf("showmap v2 ok\n");
    return 0;
}
