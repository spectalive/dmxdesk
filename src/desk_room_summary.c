#include "desk_room_summary.h"

#include "desk_rig_name.h"

struct desk_room_summary desk_room_summary(const struct desk_model *model) {
    struct desk_room_summary summary = { "—", "—", "—" };
    const char *hit_colour = NULL;
    int fog_burst = 0;
    if (model->link == DESK_LINK_READY)
        for (int i = 0; i < model->layout.placements; i++) {
            const struct desk_placement *placement = &model->layout.placement[i];
            if (placement->page != model->page || placement->bank != model->bank ||
                placement->control < 0 || placement->control >= model->count)
                continue;
            const struct desk_control *control = &model->control[placement->control];
            if (!control->enabled || control->state != DESK_ON)
                continue;
            if (placement->tile == TILE_FOG) {
                summary.smoke = control->label;
                fog_burst = 1;
            } else if (control->role == MAP_ROLE_HAZE && !fog_burst)
                summary.smoke = control->label;
            else if (placement->tile == TILE_MINI)
                summary.colour = desk_rig_name(control->label);
            else if (control->role == MAP_ROLE_BURST)
                summary.effect = control->label;
        }
    if (model->link == DESK_LINK_READY)
        for (int i = 0; i < model->layout.placements; i++) {
            const struct desk_placement *placement = &model->layout.placement[i];
            if (placement->page != model->page || placement->bank != model->bank ||
                placement->tile != TILE_MINI || placement->control < 0 ||
                placement->control >= model->count)
                continue;
            const struct desk_control *swatch = &model->control[placement->control];
            for (int j = 0; j < model->count; j++)
                if (swatch->shot_widget_id >= 0 &&
                    model->control[j].widget_id == swatch->shot_widget_id &&
                    model->control[j].kind == DESK_BURST && model->control[j].state == DESK_ON) {
                    const char *name = desk_rig_name(swatch->label);
                    hit_colour = hit_colour ? "Varios" : name;
                }
        }
    if (hit_colour)
        summary.colour = hit_colour;
    return summary;
}
