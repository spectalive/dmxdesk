// SOURCES: showmap.c showmap_release_to_parse.c showmap_release_to_prune.c
// A pick's releaseTo: the hook the room state runs in the pick's frame,
// keyed by that state's widget. Entries whose key is not a state, or whose
// value is not a hook of the pick's own solo frame, are dropped one by one
// and the map still loads; a map without the field (every map before it)
// loads with no entries at all.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "showmap.h"

#define CONTROL(key, widget, role, solo, extra) \
    "\"" key "\":{\"widget\":" #widget ",\"action\":\"toggle\",\"caption\":\"" key "\"," \
    "\"role\":\"" role "\",\"solo\":" #solo ",\"enabled\":true" extra "}"

static int build(char *buf, size_t cap, const char *pick_c_extra) {
    return snprintf(buf, cap,
        "{\"schema\":2,\"qlcVersion\":\"5.2.2\",\"show\":{\"key\":\"t\",\"workspace\":\"t.qxw\",\"sha256\":\"\"},"
        "\"pages\":[{\"key\":\"live\",\"title\":\"LIVE\",\"sections\":["
        "{\"key\":\"room\",\"title\":\"SALA\",\"solo\":3,\"controls\":[\"auto\",\"charla\"]},"
        "{\"key\":\"colour\",\"title\":\"COLOR\",\"solo\":62,\"controls\":[\"hook-a\",\"pick-a\",\"pick-b\",\"pick-c\"]},"
        "{\"key\":\"panels\",\"title\":\"PANELES\",\"solo\":89,\"controls\":[\"hook-p\"]}]}],"
        "\"controls\":{"
        CONTROL("auto", 4, "state", 3, "") ","
        CONTROL("charla", 5, "state", 3, "") ","
        CONTROL("hook-a", 64, "hook", 62, "") ","
        // 4 -> 64 is good; 5 -> 91 is a hook of another frame; 6 is no
        // control; x is no widget id; 70 is a pick, not a state; "64" is
        // not a number.
        CONTROL("pick-a", 70, "pick", 62,
                ",\"releaseTo\":{\"4\":64,\"5\":91,\"6\":64,\"x\":64,\"70\":64,\"8\":\"64\"}") ","
        // 4 -> 70 names a pick, not a hook; 5 -> 64 is good.
        CONTROL("pick-b", 71, "pick", 62, ",\"releaseTo\":{\"4\":70,\"5\":64}") ","
        CONTROL("pick-c", 72, "pick", 62, "%s") ","
        CONTROL("hook-p", 91, "hook", 89, "")
        "}}", pick_c_extra);
}

static const struct map_control *find(const struct show_map *map, const char *key) {
    for (int i = 0; i < map->count; i++)
        if (strcmp(map->control[i].key, key) == 0)
            return &map->control[i];
    assert(!"control not in the map");
    return NULL;
}

int main(void) {
    char text[4096];
    assert(build(text, sizeof text, "") < (int)sizeof text);
    struct show_map map;
    assert(showmap_parse(text, strlen(text), &map) == 0);

    const struct map_control *a = find(&map, "pick-a");
    assert(a->releases == 1);
    assert(a->release_to[0].state_widget == 4 && a->release_to[0].hook_widget == 64);

    const struct map_control *b = find(&map, "pick-b");
    assert(b->releases == 1);
    assert(b->release_to[0].state_widget == 5 && b->release_to[0].hook_widget == 64);

    // Absent: nothing to press.
    assert(find(&map, "pick-c")->releases == 0);
    assert(find(&map, "hook-a")->releases == 0);

    // Not an object: ignored, the map still loads.
    assert(build(text, sizeof text, ",\"releaseTo\":[64,65]") < (int)sizeof text);
    assert(showmap_parse(text, strlen(text), &map) == 0);
    assert(find(&map, "pick-c")->releases == 0);
    assert(find(&map, "pick-a")->releases == 1);

    // Empty object: the generator omits it, but it means nothing either way.
    assert(build(text, sizeof text, ",\"releaseTo\":{}") < (int)sizeof text);
    assert(showmap_parse(text, strlen(text), &map) == 0);
    assert(find(&map, "pick-c")->releases == 0);

    // A value on a pick outside any solo frame has no frame to share.
    assert(build(text, sizeof text, ",\"releaseTo\":{\"4\":64}") < (int)sizeof text);
    char *unsoloed = strstr(text, "\"pick-c\":{");
    assert(unsoloed);
    char *solo = strstr(unsoloed, "\"solo\":62");
    assert(solo);
    memcpy(solo, "\"solo\":-1", 9);
    assert(showmap_parse(text, strlen(text), &map) == 0);
    assert(find(&map, "pick-c")->releases == 0);
    assert(find(&map, "pick-a")->releases == 1);

    // The shipped map (Vibra, qlctool v0.1.9): 99 picks carry 432 entries,
    // and every one names a state and a hook of the pick's own frame, so the
    // pruner keeps them all.
    FILE *f = fopen("show/vibra.desk.json", "rb");
    assert(f);
    char *vibra = malloc(512 * 1024);
    assert(vibra);
    size_t len = fread(vibra, 1, 512 * 1024, f);
    fclose(f);
    static struct show_map shipped;
    assert(showmap_parse(vibra, len, &shipped) == 0);
    free(vibra);
    int carrying = 0, entries = 0;
    for (int i = 0; i < shipped.count; i++) {
        carrying += shipped.control[i].releases > 0;
        entries += shipped.control[i].releases;
    }
    assert(carrying == 99 && entries == 432);

    printf("showmap releaseTo ok\n");
    return 0;
}
