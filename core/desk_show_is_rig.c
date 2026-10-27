#include "desk_show_is_rig.h"

#include <string.h>

int desk_show_is_rig(const struct map_control *c) {
    return c->role == MAP_ROLE_PICK && strncmp(c->key, "rig-", 4) == 0 && c->swatches == 1;
}
