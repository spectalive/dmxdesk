#include "desk_tick_session.h"

#include <stdio.h>

#include "desk_apply_event.h"
#include "desk_follow_page.h"
#include "desk_link_of.h"
#include "desk_take_snapshot.h"

void desk_tick_session(struct desk_app *app, int64_t now) {
    app->link = app->master.step(app->master.ctx, now);
    desk_take_snapshot(app, now);
    struct desk_event event;
    while (app->master.recv(app->master.ctx, &event) == 1)
        desk_apply_event(&app->model, &app->speed, &event, now);
    desk_set_link(&app->model, desk_link_of(app->link, app->conf.master[0] != '\0'));
    desk_speed_set_link(&app->speed, app->link == DESK_CONSOLE_READY, now);
    desk_speed_tick(&app->speed, now);
    if (app->speed.refresh_wanted) {
        app->speed.refresh_wanted = 0;
        app->master.refresh(app->master.ctx, now);
        fprintf(stderr, "speed: no echo, re-reading the show\n");
    }
    desk_follow_page(app, now);
}
