#include "desk_show_accent_tile.h"

#include <string.h>

enum desk_tile desk_show_accent_tile(const struct map_control *c) {
    if (c->role != MAP_ROLE_ACCENT && c->role != MAP_ROLE_BURST)
        return TILE_CUE;
    // A burst names the accent it fires through its source.
    const char *key = c->role == MAP_ROLE_BURST && c->source[0] ? c->source : c->key;
    return strncmp(key, "humo", 4) == 0 ? TILE_FOG : TILE_HOLD;
}
