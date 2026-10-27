#include "desk_take_touches.h"

#include "desk_layout.h"
#include "desk_release_holds.h"
#include "desk_route_touch.h"

void desk_take_touches(struct desk_app *app, const struct desk_platform *p, int64_t now) {
    struct touch_event events[32];
    int n = p->touches(p->ctx, events, 32);
    if (n < 0) {
        desk_release_holds(&app->hold, &app->model, app->session, app->hold_owner, now);
        // The final release must reach the socket before the desk exits.
        qlc_session_step(app->session, now);
        desk_cancel_all(&app->model);
        desk_speed_touch_cancel(&app->speed);
        desk_setup_touch_cancel(&app->setup);
        app->stop = 1;
        n = 0;
    }
    for (int i = 0; i < n; i++) {
        if (events[i].kind == TOUCH_DOWN)
            desk_lock_contact(&app->lock, 1);
        else if (events[i].kind == TOUCH_UP || events[i].kind == TOUCH_CANCEL)
            desk_lock_contact(&app->lock, 0);
        if (events[i].x < 0 || events[i].x >= DESK_W ||
            events[i].y < 0 || events[i].y >= DESK_H) {
            if (events[i].kind == TOUCH_DOWN)
                continue;
            events[i].kind = TOUCH_CANCEL;
        }
        desk_route_touch(app, p, &events[i], now);
    }
}
