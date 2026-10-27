#include "desk_tick_session.h"

#include <stdio.h>

#include "desk_apply_frame.h"
#include "desk_follow_page.h"
#include "desk_link_of.h"
#include "desk_take_snapshot.h"

void desk_tick_session(struct desk_app *app, int64_t now) {
    app->link = qlc_session_step(app->session, now);
    desk_take_snapshot(app, now);
    char frame[4096];
    while (qlc_session_recv(app->session, frame, sizeof frame) == 1)
        desk_apply_frame(&app->model, &app->speed, frame, now);
    desk_set_link(&app->model, desk_link_of(app->link, app->conf.master[0] != '\0'));
    desk_speed_set_link(&app->speed, app->link == QLC_READY, now);
    desk_speed_tick(&app->speed, now);
    if (app->speed.refresh_wanted) {
        app->speed.refresh_wanted = 0;
        qlc_session_refresh(app->session, now);
        fprintf(stderr, "speed: no echo, re-reading the show\n");
    }
    desk_follow_page(app, now);
}
