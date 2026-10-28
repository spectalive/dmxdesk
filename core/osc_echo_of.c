#include "osc_echo_of.h"

#include "desk_surface_find.h"
#include "osc_command_messages.h"
#include "osc_msg_make.h"

int osc_echo_of(const struct osc_map *map, const struct desk_command *cmd, struct osc_msg *out) {
    if (cmd->kind == DESK_CMD_STOP_ALL)
        return 0;
    if (cmd->kind == DESK_CMD_TOGGLE) {
        const struct desk_surface_item *it = desk_surface_find(&map->surface, cmd->id);
        if (!it || it->kind != SURFACE_TOGGLE)
            return 0;
        return osc_msg_make(out, map->address[it - map->surface.item], 'f', 0,
                            it->on ? 0.0f : 1.0f) == 1;
    }
    struct osc_msg msgs[2];
    int n = osc_command_messages(map, cmd, msgs);
    if (n != 1 && !(n == 2 && cmd->kind == DESK_CMD_SPEED_FACTOR))
        return 0;
    *out = msgs[0];     // a multiplier button's press is what sets it
    return 1;
}
