#include "desk_follow_page.h"

#include "desk_release_holds.h"

void desk_follow_page(struct desk_app *app, int64_t now) {
    if (app->model.page == app->last_page)
        return;
    app->last_page = app->model.page;
    desk_speed_reset_taps(&app->speed);
    desk_speed_touch_cancel(&app->speed);
    app->speed_slot = -1;
    desk_release_holds(&app->hold, &app->model, app->session, app->hold_owner, now);
}
