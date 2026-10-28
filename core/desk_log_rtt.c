#include "desk_log_rtt.h"

#include <stdio.h>

void desk_log_rtt(struct desk_app *app) {
    if (!app->log_rtt || app->link != DESK_CONSOLE_READY ||
        app->master.last_rtt(app->master.ctx) == app->last_rtt_logged)
        return;
    app->last_rtt_logged = app->master.last_rtt(app->master.ctx);
    if (app->last_rtt_logged >= 0)
        printf("rtt %d\n", app->last_rtt_logged);
}
