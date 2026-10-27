#include "desk_take_power_key.h"

#include <stdio.h>

#include "desk_release_holds.h"

void desk_take_power_key(struct desk_app *app, const struct desk_platform *p, int64_t now) {
    desk_lock_power_key(&app->lock, now);
    desk_cancel_all(&app->model);
    desk_speed_touch_cancel(&app->speed);
    desk_setup_touch_cancel(&app->setup);
    desk_input_cancel_all(&app->input);
    desk_release_holds(&app->hold, &app->model, app->session, app->hold_owner, now);
    app->gear_slot = app->setup_slot = app->speed_slot = -1;
    if (app->setup.open) {
        desk_setup_close(&app->setup);
        app->bar_ready = 0;
        app->last_status_ms = 0;
    }
    // The fingers the display took with it are lifted for the lock's count
    // too, or nothing would count after the wake.
    int gone = p->drop_contacts(p->ctx);
    for (int d = 0; d < gone; d++)
        desk_lock_contact(&app->lock, 0);
    if (app->lock.state == DESK_BLANKED) {
        p->display(p->ctx, 0);
    } else {
        p->display(p->ctx, 1);
        app->model.dirty = 1;
    }
    fprintf(stderr, "desk: power key, surface %s\n",
            app->lock.state == DESK_BLANKED ? "blanked" : "locked");
}
