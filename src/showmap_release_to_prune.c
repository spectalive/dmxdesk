#include "showmap_release_to_prune.h"

#include <stdio.h>

static const struct map_control *by_widget(const struct show_map *map, int widget) {
    for (int i = 0; i < map->count; i++)
        if (map->control[i].widget_id == widget)
            return &map->control[i];
    return NULL;
}

void showmap_release_to_prune(struct show_map *map) {
    for (int i = 0; i < map->count; i++) {
        struct map_control *c = &map->control[i];
        int kept = 0;
        for (int e = 0; e < c->releases; e++) {
            struct map_release r = c->release_to[e];
            const struct map_control *state = by_widget(map, r.state_widget);
            const struct map_control *hook = by_widget(map, r.hook_widget);
            if (!state || state->role != MAP_ROLE_STATE || !hook || hook->role != MAP_ROLE_HOOK ||
                c->solo_id < 0 || hook->solo_id != c->solo_id) {
                fprintf(stderr, "map: control %s: releaseTo %d -> %d is not a state and a hook "
                        "of its frame, ignored\n", c->key, r.state_widget, r.hook_widget);
                continue;
            }
            c->release_to[kept++] = r;
        }
        c->releases = kept;
    }
}
