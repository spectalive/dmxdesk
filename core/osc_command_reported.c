#include "osc_command_reported.h"

#include "desk_surface_find.h"

int osc_command_reported(const struct osc_map *map, const struct desk_command *cmd) {
    if (cmd->kind == DESK_CMD_GRAND_MASTER)
        return map->grand_master < 0 ? map->feedback : map->reports[map->grand_master];
    if (cmd->kind == DESK_CMD_FUNCTION)
        return map->feedback;
    const struct desk_surface_item *it = desk_surface_find(&map->surface, cmd->id);
    return it ? map->reports[it - map->surface.item] : map->feedback;
}
