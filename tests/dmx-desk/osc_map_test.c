// SOURCES: osc_map_load.c osc_map_parse.c osc_map_parse_items.c osc_map_parse_functions.c osc_item_parse.c osc_function_parse.c osc_kind_of.c osc_json_int.c osc_json_bool.c osc_json_address.c osc_map_free.c desk_surface_free.c desk_surface_find.c desk_surface_is_button.c osc_map_target.c osc_map_apply.c osc_clamp.c osc_msg_number.c osc_event_of.c osc_command_messages.c osc_echo_of.c osc_msg_make.c
// A console's OSC surface file (2026-10-28): what it declares reaches the
// desk's surface field for field, each command becomes the messages a
// console reads, what the console says back becomes the desk's events, and
// a wrong file is refused with its reason instead of half loaded.
#include <assert.h>
#include <string.h>

#include "desk_surface_find.h"
#include "osc_command_messages.h"
#include "osc_echo_of.h"
#include "osc_event_of.h"
#include "osc_map_apply.h"
#include "osc_map_free.h"
#include "osc_map_load.h"
#include "osc_map_parse.h"

static struct osc_msg msg(const char *address, char type, int i, float f) {
    struct osc_msg m;
    memset(&m, 0, sizeof m);
    strcpy(m.address, address);
    m.type = type;
    m.i = i;
    m.f = f;
    return m;
}

static struct desk_event heard(struct osc_map *map, struct osc_msg m) {
    return osc_event_of(map, osc_map_apply(map, &m));
}

static int refused(const char *json) {
    struct osc_map map;
    char why[128] = "";
    int rc = osc_map_parse(json, strlen(json), &map, why, sizeof why);
    if (rc == 0) {
        osc_map_free(&map);
        return 0;
    }
    assert(why[0] && map.surface.item == NULL && map.address == NULL);
    return 1;
}

int main(void) {
    struct osc_map map;
    char why[128];
    assert(osc_map_load("tests/dmx-desk/fixtures/osc-surface.json", &map, why, sizeof why) == 0);
    const struct desk_surface *s = &map.surface;
    assert(s->count == 9 && s->version[0] == '\0' && !map.feedback && map.functions == 2);
    const struct desk_surface_item *group = desk_surface_find(s, 1);
    assert(group && group->kind == SURFACE_GROUP && group->group_id == -1 && group->function_id == -1);
    const struct desk_surface_item *toggle = desk_surface_find(s, 2);
    assert(toggle->kind == SURFACE_TOGGLE && toggle->group_id == 1 && toggle->function_id == 5);
    assert(!toggle->on && !toggle->disabled);
    const struct desk_surface_item *flash = desk_surface_find(s, 3);
    assert(flash->kind == SURFACE_FLASH && flash->flash_max_ms == 2000 && flash->flash_priority);
    assert(desk_surface_find(s, 4)->kind == SURFACE_STOP_ALL);
    const struct desk_surface_item *gm = desk_surface_find(s, 9);
    assert(gm->kind == SURFACE_LEVEL && gm->level == 255 && map.grand_master == gm - s->item);
    const struct desk_surface_item *dial = desk_surface_find(s, 12);
    assert(dial->kind == SURFACE_DIAL && dial->dial_ms == 500 && dial->dial_factor == 3 &&
           dial->dial_max_ms == 10000);
    assert(desk_surface_find(s, 13)->dial_max_ms == -1 && desk_surface_find(s, 13)->dial_factor == -1);
    assert(toggle->dial_ms == -1 && toggle->dial_factor == -1);
    assert(desk_surface_find(s, 14)->kind == SURFACE_BUTTON && desk_surface_find(s, 14)->disabled);
    assert(desk_surface_find(s, 20)->kind == SURFACE_OTHER);

    // Commands as messages: a press is 1 then 0 on the button's address.
    struct osc_msg out[2];
    struct desk_command cmd = { DESK_CMD_TOGGLE, 2, 0 };
    assert(osc_command_messages(&map, &cmd, out) == 2);
    assert(!strcmp(out[0].address, "/x/2") && out[0].type == 'f' && out[0].f == 1.0f);
    assert(!strcmp(out[1].address, "/x/2") && out[1].f == 0.0f);
    cmd = (struct desk_command){ DESK_CMD_STOP_ALL, 4, 0 };
    assert(osc_command_messages(&map, &cmd, out) == 2 && !strcmp(out[0].address, "/x/stop"));
    cmd = (struct desk_command){ DESK_CMD_FLASH, 3, 1 };
    assert(osc_command_messages(&map, &cmd, out) == 1 && out[0].f == 1.0f);
    cmd.value = 0;
    assert(osc_command_messages(&map, &cmd, out) == 1 && out[0].f == 0.0f);
    cmd = (struct desk_command){ DESK_CMD_FUNCTION, 40, 1 };
    assert(osc_command_messages(&map, &cmd, out) == 1 && !strcmp(out[0].address, "/f/40") &&
           out[0].f == 1.0f);
    cmd = (struct desk_command){ DESK_CMD_GRAND_MASTER, 0, 255 };
    assert(osc_command_messages(&map, &cmd, out) == 1 && !strcmp(out[0].address, "/x/gm") &&
           out[0].f == 1.0f);
    cmd = (struct desk_command){ DESK_CMD_SPEED_MS, 12, 700 };
    assert(osc_command_messages(&map, &cmd, out) == 1 && !strcmp(out[0].address, "/x/12/ms") &&
           out[0].type == 'i' && out[0].i == 700);
    cmd = (struct desk_command){ DESK_CMD_SPEED_FACTOR, 12, 4 };
    assert(osc_command_messages(&map, &cmd, out) == 1 &&
           !strcmp(out[0].address, "/x/12/factor") && out[0].i == 4);

    // What the map cannot address is dropped, never sent somewhere else.
    struct desk_command wrong[] = {
        { DESK_CMD_TOGGLE, 99, 0 },         // no such control
        { DESK_CMD_TOGGLE, 12, 0 },         // a dial is not a button
        { DESK_CMD_TOGGLE, 1, 0 },          // nor is a group
        { DESK_CMD_FLASH, 9, 1 },           // nor a fader
        { DESK_CMD_SPEED_MS, 2, 100 },      // a button is not a dial
        { DESK_CMD_SPEED_FACTOR, 13, 2 },   // a dial with no factorAddress
        { DESK_CMD_FUNCTION, 5, 1 },        // a function with no address of its own
    };
    for (size_t i = 0; i < sizeof wrong / sizeof wrong[0]; i++)
        assert(osc_command_messages(&map, &wrong[i], out) == -1);

    // What a console without feedback would say back.
    struct osc_msg echo;
    cmd = (struct desk_command){ DESK_CMD_TOGGLE, 2, 0 };
    assert(osc_echo_of(&map, &cmd, &echo) == 1 && !strcmp(echo.address, "/x/2") && echo.f == 1.0f);
    cmd = (struct desk_command){ DESK_CMD_STOP_ALL, 4, 0 };
    assert(osc_echo_of(&map, &cmd, &echo) == 0);
    cmd = (struct desk_command){ DESK_CMD_TOGGLE, 14, 0 };
    assert(osc_echo_of(&map, &cmd, &echo) == 0);    // a plain button has no state to echo
    cmd = (struct desk_command){ DESK_CMD_SPEED_MS, 12, 800 };
    assert(osc_echo_of(&map, &cmd, &echo) == 1 && echo.type == 'i' && echo.i == 800);

    // What the console says, as the desk's events and the surface's state.
    struct desk_event e = heard(&map, msg("/x/2", 'f', 0, 1.0f));
    assert(e.kind == DESK_EVENT_FUNCTION && e.id == 5 && e.running == 1 && toggle->on == 1);
    e = heard(&map, msg("/x/2", 'i', 0, 0));
    assert(e.kind == DESK_EVENT_FUNCTION && e.running == 0 && toggle->on == 0);
    e = heard(&map, msg("/x/gm", 'f', 0, 0.5f));
    assert(e.kind == DESK_EVENT_GRAND_MASTER && e.value == 128 && gm->level == 128);
    e = heard(&map, msg("/x/gm", 'i', 200, 0));
    assert(e.kind == DESK_EVENT_GRAND_MASTER && e.value == 200);
    e = heard(&map, msg("/x/gm", 'f', 0, 7.0f));
    assert(e.value == 255);
    e = heard(&map, msg("/x/12/ms", 'i', 700, 0));
    assert(e.kind == DESK_EVENT_SPEED && e.id == 12 && e.value == 700 && e.factor == 3);
    e = heard(&map, msg("/x/12/factor", 'i', 4, 0));
    assert(e.kind == DESK_EVENT_SPEED && e.id == 12 && e.value == 700 && e.factor == 4);
    e = heard(&map, msg("/x/12/factor", 'i', 40, 0));
    assert(e.factor == 10);
    e = heard(&map, msg("/f/6", 'i', 1, 0));
    assert(e.kind == DESK_EVENT_FUNCTION && e.id == 6 && e.running == 1);
    e = heard(&map, msg("/x/14", 'f', 0, 1.0f));
    assert(e.kind == DESK_EVENT_NONE);              // a button driving no function
    e = heard(&map, msg("/nobody", 'f', 0, 1.0f));
    assert(e.kind == DESK_EVENT_NONE);
    e = heard(&map, msg("", 0, 0, 0));
    assert(e.kind == DESK_EVENT_NONE);
    osc_map_free(&map);
    assert(map.surface.count == 0 && map.address == NULL && map.grand_master == -1);

    // Wrong files, each refused with a reason.
    assert(osc_map_load("/nonexistent/osc.json", &map, why, sizeof why) == -1 && why[0]);
    assert(!refused("{\"osc\":1,\"items\":[]}"));
    assert(refused("not json"));
    assert(refused("[]"));
    assert(refused("{\"items\":[]}"));
    assert(refused("{\"osc\":2,\"items\":[]}"));
    assert(refused("{\"osc\":1}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":2,\"kind\":\"group\"},{\"id\":1,\"kind\":\"group\"}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1,\"kind\":\"group\"},{\"id\":1,\"kind\":\"group\"}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1,\"kind\":\"knob\"}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"kind\":\"group\"}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1.5,\"kind\":\"group\"}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1,\"kind\":\"toggle\"}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1,\"kind\":\"toggle\",\"address\":\"x/1\"}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1,\"kind\":\"toggle\",\"address\":\"/1\",\"factorAddress\":\"/2\"}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1,\"kind\":\"level\",\"address\":\"/1\",\"level\":256}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1,\"kind\":\"dial\",\"address\":\"/1\",\"factor\":256}]}"));
    assert(refused("{\"osc\":1,\"items\":[{\"id\":1,\"kind\":\"toggle\",\"address\":\"/1\",\"on\":1}]}"));
    assert(refused("{\"osc\":1,\"grandMaster\":1,\"items\":[{\"id\":1,\"kind\":\"toggle\",\"address\":\"/1\"}]}"));
    assert(refused("{\"osc\":1,\"grandMaster\":7,\"items\":[]}"));
    assert(refused("{\"osc\":1,\"version\":\"a version far too long for it\",\"items\":[]}"));
    assert(refused("{\"osc\":1,\"items\":[],\"functions\":[{\"id\":1}]}"));
    assert(refused("{\"osc\":1,\"items\":[],\"functions\":{}}"));
    return 0;
}
