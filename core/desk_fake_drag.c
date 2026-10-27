#include "desk_fake_drag.h"

void desk_fake_drag(struct desk_model *model, int64_t now) {
    for (int i = 0; i < model->count; i++)
        if (model->control[i].kind == DESK_MASTER) {
            model->control[i].pressed = 1;
            model->control[i].requested_level = (int)((now / 8) % 256);
            const struct desk_placement *mp = desk_placement_of(model, i);
            if (mp)
                desk_damage_rect(model, mp->x, mp->y, mp->w, mp->h);
        }
}
