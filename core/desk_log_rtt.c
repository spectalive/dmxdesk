#include "desk_log_rtt.h"

#include <stdio.h>

void desk_log_rtt(struct desk_app *app) {
    if (!app->log_rtt || app->link != QLC_READY ||
        qlc_session_last_rtt(app->session) == app->last_rtt_logged)
        return;
    app->last_rtt_logged = qlc_session_last_rtt(app->session);
    if (app->last_rtt_logged >= 0)
        printf("rtt %d\n", app->last_rtt_logged);
}
