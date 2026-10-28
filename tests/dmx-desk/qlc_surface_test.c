// SOURCES: vcjson.c qlc_surface_of.c qlc_surface_kind.c desk_surface_find.c desk_surface_free.c desk_surface_is_button.c
// QLC+'s document as the desk's surface (2026-10-28): every widget of the
// real Vibra /vc.json keeps its id and its order, its kind follows its type
// and its button action, and what the desk reads comes across unchanged. A
// flash takes priority only when it both overrides and forces LTP.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desk_surface_find.h"
#include "desk_surface_free.h"
#include "desk_surface_is_button.h"
#include "qlc_surface_of.h"
#include "vcjson.h"

static char *read_fixture(const char *path, size_t *len) {
    FILE *f = fopen(path, "rb");
    assert(f);
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc((size_t)n + 1);
    assert(buf && fread(buf, 1, (size_t)n, f) == (size_t)n);
    buf[n] = '\0';
    fclose(f);
    *len = (size_t)n;
    return buf;
}

static enum desk_surface_kind expected_kind(const struct vc_widget *w) {
    if (w->type_id == VC_BUTTON)
        return w->action_type == VC_TOGGLE ? SURFACE_TOGGLE
             : w->action_type == VC_FLASH ? SURFACE_FLASH
             : w->action_type == VC_STOP_ALL ? SURFACE_STOP_ALL : SURFACE_BUTTON;
    if (w->type_id == VC_SLIDER)
        return SURFACE_LEVEL;
    if (w->type_id == VC_SPEED_DIAL)
        return SURFACE_DIAL;
    if (w->type_id == VC_FRAME || w->type_id == VC_SOLO_FRAME)
        return SURFACE_GROUP;
    return SURFACE_OTHER;
}

int main(void) {
    size_t len;
    char *json = read_fixture("tests/dmx-desk/fixtures/vc-vibra.json", &len);
    struct vc_doc doc;
    assert(vc_parse(json, len, &doc) == 0);
    free(json);

    struct desk_surface s;
    assert(qlc_surface_of(&doc, &s) == 0);
    assert(s.count == doc.count && s.count > 100);
    assert(strcmp(s.version, doc.app_version) == 0);
    int kinds[SURFACE_DIAL + 1] = { 0 };
    for (int i = 0; i < doc.count; i++) {
        const struct vc_widget *w = &doc.widget[i];
        const struct desk_surface_item *it = &s.item[i];
        assert(it->id == w->id);
        assert(i == 0 || s.item[i - 1].id < it->id);
        assert(desk_surface_find(&s, w->id) == it);
        assert(it->kind == expected_kind(w));
        assert(desk_surface_is_button(it->kind) == (w->type_id == VC_BUTTON));
        assert(it->group_id == w->parent_id && it->function_id == w->function_id);
        assert(it->disabled == w->disabled && it->on == w->state && it->level == w->value);
        assert(it->flash_max_ms == w->flash_max_hold_ms);
        assert(it->flash_priority == (w->flash_override && w->flash_force_ltp));
        assert(it->dial_ms == w->speed_ms && it->dial_factor == w->speed_factor &&
               it->dial_max_ms == w->speed_max_ms);
        kinds[it->kind]++;
    }
    // The show has every kind the desk validates against.
    assert(kinds[SURFACE_TOGGLE] > 0 && kinds[SURFACE_FLASH] > 0 && kinds[SURFACE_STOP_ALL] > 0);
    assert(kinds[SURFACE_LEVEL] > 0 && kinds[SURFACE_DIAL] > 0 && kinds[SURFACE_GROUP] > 0);
    assert(desk_surface_find(&s, -12345) == NULL);

    // Priority needs both flags.
    struct vc_widget one;
    memset(&one, 0, sizeof one);
    one.id = 7;
    one.type_id = VC_BUTTON;
    one.action_type = VC_FLASH;
    one.flash_override = 1;
    struct vc_doc single = { "5.2.2", 1, 1, &one };
    struct desk_surface t;
    assert(qlc_surface_of(&single, &t) == 0 && t.item[0].flash_priority == 0);
    desk_surface_free(&t);
    one.flash_force_ltp = 1;
    assert(qlc_surface_of(&single, &t) == 0 && t.item[0].flash_priority == 1);
    desk_surface_free(&t);
    assert(t.count == 0 && t.item == NULL);

    // An empty surface finds nothing: the desk before its first snapshot.
    struct desk_surface empty;
    memset(&empty, 0, sizeof empty);
    assert(desk_surface_find(&empty, 0) == NULL);

    desk_surface_free(&s);
    vc_free(&doc);
    return 0;
}
