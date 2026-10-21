#include "desk_paint_room_status.h"

#include "desk_layout.h"
#include "desk_paint_text_at.h"
#include "desk_rig_name.h"

void desk_paint_room_status(struct canvas *canvas, const struct desk_model *model,
                            int x, int y, int width, const struct desk_fonts *fonts) {
    const char *effect = "—", *smoke = "—", *colour = "—", *hit_colour = NULL;
    int fog_burst = 0;
    if (model->link == DESK_LINK_READY)
        for (int i = 0; i < model->layout.placements; i++) {
            const struct desk_placement *p = &model->layout.placement[i];
            if (p->page != model->page || p->bank != model->bank || p->control < 0 ||
                p->control >= model->count)
                continue;
            const struct desk_control *control = &model->control[p->control];
            if (!control->enabled || control->state != DESK_ON)
                continue;
            if (p->tile == TILE_FOG) {
                smoke = control->label;
                fog_burst = 1;
            } else if (control->role == MAP_ROLE_HAZE && !fog_burst)
                smoke = control->label;
            else if (p->tile == TILE_MINI)
                colour = desk_rig_name(control->label);
            else if (control->role == MAP_ROLE_BURST)
                effect = control->label;
        }
    if (model->link == DESK_LINK_READY)
        for (int i = 0; i < model->layout.placements; i++) {
            const struct desk_placement *p = &model->layout.placement[i];
            if (p->page != model->page || p->bank != model->bank || p->tile != TILE_MINI ||
                p->control < 0 || p->control >= model->count)
                continue;
            const struct desk_control *swatch = &model->control[p->control];
            for (int j = 0; j < model->count; j++)
                if (swatch->shot_widget_id >= 0 &&
                    model->control[j].widget_id == swatch->shot_widget_id &&
                    model->control[j].kind == DESK_BURST && model->control[j].state == DESK_ON) {
                    const char *name = desk_rig_name(swatch->label);
                    hit_colour = hit_colour ? "Varios" : name;
                }
        }
    if (hit_colour)
        colour = hit_colour;
    const char *labels[3] = { "EFECTO", "HUMO", "COLOR" };
    const char *values[3] = { effect, smoke, colour };
    for (int i = 0; i < 3; i++) {
        int row = y + i * 26;
        desk_paint_text_at(canvas, fonts->small, x, row, 60, labels[i], DESK_HERO_SOFT);
        desk_paint_text_at(canvas, fonts->tile_s, x + 62, row - 1, width - 62, values[i], DESK_INK);
    }
}
