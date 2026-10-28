#include "desk_touch_owned.h"

#include "desk_send_hold.h"

int desk_touch_owned(struct desk_app *app, const struct touch_event *e, int64_t now) {
    int slot = e->slot;
    if (slot < 0 || slot >= TOUCH_MAX_SLOTS || app->hold_owner[slot] < 0)
        return 0;
    int ci = app->hold_owner[slot];
    if (e->kind == TOUCH_UP || e->kind == TOUCH_CANCEL) {
        struct desk_control *hc = &app->model.control[ci];
        if ((hc->kind == DESK_HOLD || hc->kind == DESK_BURST) && hc->hold_index >= 0)
            desk_send_hold(&app->hold, &app->master,
                           desk_hold_release(&app->hold, hc->hold_index, slot, now), now);
        desk_set_hold_progress(&app->model, ci, -1, 0);
        app->hold_owner[slot] = -1;
    }
    return 1;
}
