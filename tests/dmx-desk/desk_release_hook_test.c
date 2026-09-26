// SOURCES: desk_release_hook.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c desk_model.c desk_master_level_at.c
// Which hook a pick's release presses: the one its releaseTo names for the
// room state that is on, only when the tap turns the pick off, and only when
// that hook is not already running (a Toggle pressed while running stops).
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_model.h"
#include "desk_release_hook.h"
#include "showmap.h"

static const char MAP[] =
    "{\"schema\":2,\"qlcVersion\":\"5.2.2\",\"show\":{\"key\":\"t\",\"workspace\":\"t.qxw\",\"sha256\":\"\"},"
    "\"pages\":[{\"key\":\"live\",\"title\":\"LIVE\",\"sections\":["
    "{\"key\":\"room\",\"title\":\"SALA\",\"solo\":3,\"controls\":[\"auto\",\"charla\",\"fiesta\"]},"
    "{\"key\":\"colour\",\"title\":\"COLOR\",\"solo\":62,\"controls\":[\"colores\",\"luz-charla\",\"rig-rojo\",\"rig-azul\"]}]}],"
    "\"controls\":{"
    "\"auto\":{\"widget\":4,\"action\":\"toggle\",\"caption\":\"AUTO\",\"role\":\"state\",\"solo\":3,\"enabled\":true},"
    "\"charla\":{\"widget\":5,\"action\":\"toggle\",\"caption\":\"CHARLA\",\"role\":\"state\",\"solo\":3,\"enabled\":true},"
    "\"fiesta\":{\"widget\":7,\"action\":\"toggle\",\"caption\":\"FIESTA\",\"role\":\"state\",\"solo\":3,\"enabled\":true},"
    "\"colores\":{\"widget\":64,\"action\":\"toggle\",\"caption\":\"COLORES\",\"role\":\"hook\",\"solo\":62,\"enabled\":true},"
    "\"luz-charla\":{\"widget\":69,\"action\":\"toggle\",\"caption\":\"LUZ\",\"role\":\"hook\",\"solo\":62,\"enabled\":true},"
    "\"rig-rojo\":{\"widget\":70,\"action\":\"toggle\",\"caption\":\"ROJO\",\"role\":\"pick\",\"solo\":62,\"enabled\":true,"
    "\"releaseTo\":{\"4\":64,\"5\":69}},"
    "\"rig-azul\":{\"widget\":80,\"action\":\"toggle\",\"caption\":\"AZUL\",\"role\":\"pick\",\"solo\":62,\"enabled\":true}"
    "}}";

// The model as showmap_build would leave it: one cue per map control, in
// map order, with the master's word on each state set by the caller.
static void model_of(struct desk_model *model, const struct show_map *map) {
    desk_init(model);
    for (int i = 0; i < map->count; i++) {
        struct desk_control c;
        memset(&c, 0, sizeof c);
        c.kind = DESK_CUE;
        c.widget_id = map->control[i].widget_id;
        c.function_id = 100 + i;
        c.role = map->control[i].role;
        c.enabled = 1;
        assert(desk_add(model, &c) == i);
        model->control[i].state = DESK_OFF;
    }
}

static void set_state(struct desk_model *model, int widget, enum desk_state state) {
    for (int i = 0; i < model->count; i++)
        if (model->control[i].widget_id == widget)
            model->control[i].state = state;
}

int main(void) {
    struct show_map map;
    assert(showmap_parse(MAP, strlen(MAP), &map) == 0);
    struct desk_model model;
    model_of(&model, &map);

    // AUTO runs and ROJO is latched: releasing it returns the frame to COLORES.
    set_state(&model, 4, DESK_ON);
    set_state(&model, 70, DESK_ON);
    assert(desk_release_hook(&model, &map, 70) == 64);

    // CHARLA runs instead: its own hook.
    set_state(&model, 4, DESK_OFF);
    set_state(&model, 5, DESK_ON);
    assert(desk_release_hook(&model, &map, 70) == 69);

    // FIESTA has no entry: nothing.
    set_state(&model, 5, DESK_OFF);
    set_state(&model, 7, DESK_ON);
    assert(desk_release_hook(&model, &map, 70) == -1);

    // No state on at all: nothing.
    set_state(&model, 7, DESK_OFF);
    assert(desk_release_hook(&model, &map, 70) == -1);

    // The hook already runs: pressing it would stop it.
    set_state(&model, 4, DESK_ON);
    set_state(&model, 64, DESK_ON);
    assert(desk_release_hook(&model, &map, 70) == -1);
    set_state(&model, 64, DESK_OFF);
    assert(desk_release_hook(&model, &map, 70) == 64);

    // The hook's state is not known: nothing is pressed blind.
    set_state(&model, 64, DESK_UNKNOWN);
    assert(desk_release_hook(&model, &map, 70) == -1);
    set_state(&model, 64, DESK_OFF);

    // A tap that turns ROJO on is not a release.
    set_state(&model, 70, DESK_OFF);
    assert(desk_release_hook(&model, &map, 70) == -1);
    set_state(&model, 70, DESK_UNKNOWN);
    assert(desk_release_hook(&model, &map, 70) == -1);

    // A pick without releaseTo, released, presses nothing.
    set_state(&model, 80, DESK_ON);
    assert(desk_release_hook(&model, &map, 80) == -1);

    // A widget the map does not have presses nothing.
    assert(desk_release_hook(&model, &map, 999) == -1);

    // A hook the console could not back (disabled) is never pressed.
    set_state(&model, 70, DESK_ON);
    assert(desk_release_hook(&model, &map, 70) == 64);
    for (int i = 0; i < model.count; i++)
        if (model.control[i].widget_id == 64)
            model.control[i].enabled = 0;
    assert(desk_release_hook(&model, &map, 70) == -1);

    printf("desk release hook ok\n");
    return 0;
}
