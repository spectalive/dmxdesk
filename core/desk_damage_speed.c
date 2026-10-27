#include "desk_damage_speed.h"

#include "desk_speed_layout.h"

void desk_damage_speed(struct desk_app *app) {
    if (app->speed.dirty && app->layout.speed_page >= 0 && app->model.page == app->layout.speed_page) {
        desk_damage_rect(&app->model, SPEED_CARD_X(0), SPEED_CARD_Y,
                         SPEED_BOTH_X + SPEED_BOTH_W - SPEED_CARD_X(0),
                         SPEED_BOTH_Y + SPEED_BOTH_H - SPEED_CARD_Y);
    }
    app->speed.dirty = 0;
}
