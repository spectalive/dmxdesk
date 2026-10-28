#include "osc_command_messages.h"

#include "desk_surface_find.h"
#include "desk_surface_is_button.h"
#include "osc_msg_make.h"

int osc_command_messages(const struct osc_map *map, const struct desk_command *cmd,
                         struct osc_msg out[2]) {
    if (cmd->kind == DESK_CMD_GRAND_MASTER)
        return map->grand_master < 0 ? -1
             : osc_msg_make(out, map->address[map->grand_master], 'f', 0, (float)cmd->value / 255.0f);
    if (cmd->kind == DESK_CMD_FUNCTION) {
        for (int i = 0; i < map->functions; i++)
            if (map->function[i].id == cmd->id)
                return osc_msg_make(out, map->function[i].address, 'f', 0, cmd->value ? 1.0f : 0.0f);
        return -1;
    }
    const struct desk_surface_item *it = desk_surface_find(&map->surface, cmd->id);
    if (!it)
        return -1;
    int at = (int)(it - map->surface.item);
    switch (cmd->kind) {
    case DESK_CMD_TOGGLE:
    case DESK_CMD_STOP_ALL:
        if (!desk_surface_is_button(it->kind) || osc_msg_make(&out[0], map->address[at], 'f', 0, 1.0f) < 0)
            return -1;
        osc_msg_make(&out[1], map->address[at], 'f', 0, 0.0f);
        return 2;
    case DESK_CMD_FLASH:
        return desk_surface_is_button(it->kind)
             ? osc_msg_make(out, map->address[at], 'f', 0, cmd->value ? 1.0f : 0.0f) : -1;
    case DESK_CMD_SPEED_MS:
        return it->kind == SURFACE_DIAL ? osc_msg_make(out, map->address[at], 'i', cmd->value, 0) : -1;
    case DESK_CMD_SPEED_FACTOR:
        return it->kind == SURFACE_DIAL ? osc_msg_make(out, map->factor_address[at], 'i', cmd->value, 0) : -1;
    default:
        return -1;
    }
}
