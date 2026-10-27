#include "desk_tick_holds.h"

#include "desk_send_hold.h"

void desk_tick_holds(struct desk_app *app, int64_t now) {
    struct desk_model *model = &app->model;
    struct hold_action out[MAP_MAX_CONTROLS];
    int n = desk_hold_tick(&app->hold, now, out, MAP_MAX_CONTROLS);
    for (int k = 0; k < n; k++)
        desk_send_hold(&app->hold, app->session, out[k], now);
    desk_hold_set_link(&app->hold, app->link == QLC_READY);
    if (app->link == QLC_READY) {
        n = desk_hold_owed(&app->hold, out, MAP_MAX_CONTROLS);
        for (int k = 0; k < n; k++)
            desk_send_hold(&app->hold, app->session, out[k], now);
    }
    for (int i = 0; i < model->count; i++) {
        struct desk_control *c = &model->control[i];
        if ((c->kind != DESK_HOLD && c->kind != DESK_BURST) || c->hold_index < 0 || c->pressed)
            continue;
        int unresolved = desk_hold_unresolved(&app->hold, c->hold_index);
        desk_set_hold_progress(model, i, unresolved ? -2 : -1, 0);
    }
    for (int s = 0; s < TOUCH_MAX_SLOTS; s++) {
        int owner = app->hold_owner[s];
        if (owner < 0 || (model->control[owner].kind != DESK_HOLD &&
                          model->control[owner].kind != DESK_BURST))
            continue;
        int hi = model->control[owner].hold_index;
        int progress = hi >= 0 ? desk_hold_progress(&app->hold, hi, now) : -1;
        if (progress >= 0)
            progress = (progress / 50) * 50;    // twenty steps, not a repaint per millisecond
        desk_set_hold_progress(model, owner, progress, 1);
    }
}
