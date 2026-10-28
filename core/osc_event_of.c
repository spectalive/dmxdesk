#include "osc_event_of.h"

#include "desk_surface_is_button.h"

struct desk_event osc_event_of(const struct osc_map *map, struct osc_target target) {
    struct desk_event e = { DESK_EVENT_NONE, -1, 0, 0, 0 };
    if (target.kind == OSC_TARGET_FUNCTION) {
        e.kind = DESK_EVENT_FUNCTION;
        e.id = map->function[target.index].id;
        e.running = map->function[target.index].running;
        return e;
    }
    if (target.kind == OSC_TARGET_NONE)
        return e;
    const struct desk_surface_item *it = &map->surface.item[target.index];
    if (it->kind == SURFACE_DIAL) {
        e.kind = DESK_EVENT_SPEED;
        e.id = it->id;
        e.value = it->dial_ms;
        e.factor = it->dial_factor;
    } else if (it->kind == SURFACE_LEVEL && target.index == map->grand_master) {
        e.kind = DESK_EVENT_GRAND_MASTER;
        e.value = it->level;
    } else if (desk_surface_is_button(it->kind) && it->function_id >= 0) {
        e.kind = DESK_EVENT_FUNCTION;
        e.id = it->function_id;
        e.running = it->on;
    }
    return e;
}
