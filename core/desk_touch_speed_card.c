#include "desk_touch_speed_card.h"

#include "desk_send_speed.h"

int desk_touch_speed_card(struct desk_app *app, const struct desk_platform *p,
                          const struct touch_event *e, int64_t now) {
    if (app->layout.speed_page < 0 || app->model.page != app->layout.speed_page ||
        e->slot != app->speed_slot)
        return 0;
    if (e->kind == TOUCH_UP) {
        // A release after the lock fires nothing.
        struct speed_action sa = desk_speed_touch_up(&app->speed, (int)e->x, (int)e->y,
                                                     p->now(p->ctx));
        if (desk_lock_allows(&app->lock))
            desk_send_speed(app->session, sa, now);
        app->speed_slot = -1;
    } else if (e->kind == TOUCH_CANCEL) {
        desk_speed_touch_cancel(&app->speed);
        app->speed_slot = -1;
    }
    return 1;
}
