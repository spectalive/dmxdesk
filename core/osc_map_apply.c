#include "osc_map_apply.h"

#include "desk_surface_is_button.h"
#include "osc_clamp.h"
#include "osc_map_target.h"
#include "osc_msg_number.h"

struct osc_target osc_map_apply(struct osc_map *map, const struct osc_msg *msg) {
    struct osc_target t = osc_map_target(map, msg->address);
    float number = osc_msg_number(msg);
    if (t.kind == OSC_TARGET_FUNCTION) {
        map->function[t.index].running = number > 0.5f;
        return t;
    }
    if (t.kind == OSC_TARGET_NONE)
        return t;
    struct desk_surface_item *it = &map->surface.item[t.index];
    if (t.kind == OSC_TARGET_FACTOR)
        it->dial_factor = osc_clamp(number, 0, 10);
    else if (desk_surface_is_button(it->kind))
        it->on = number > 0.5f;
    else if (it->kind == SURFACE_LEVEL)
        it->level = osc_clamp(msg->type == 'f' ? number * 255.0f : number, 0, 255);
    else if (it->kind == SURFACE_DIAL)
        it->dial_ms = osc_clamp(number, 0, 0x7fffffff);
    return t;
}
