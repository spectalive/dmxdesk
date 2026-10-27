#include "desk_take_frame_damage.h"

#include "desk_setup_layout.h"

int desk_take_frame_damage(struct desk_app *app, int *dx, int *dy, int *dw, int *dh) {
    if (app->lock.state == DESK_BLANKED)
        app->model.dirty = 0;
    if (app->setup.dirty) {
        app->setup.dirty = 0;
        desk_damage_rect(&app->model, SETUP_SHEET_X, SETUP_SHEET_Y, SETUP_SHEET_W, SETUP_SHEET_H);
    }
    return desk_take_damage(&app->model, dx, dy, dw, dh);
}
