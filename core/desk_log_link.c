#include "desk_log_link.h"

#include <stdio.h>
#include <string.h>

void desk_log_link(struct desk_app *app, int64_t now) {
    enum qlc_link link = app->link;
    if (link == app->last_link && strcmp(app->last_reason, qlc_session_reason(app->session)) == 0)
        return;
    app->last_link = link;
    snprintf(app->last_reason, sizeof app->last_reason, "%s", qlc_session_reason(app->session));
    fprintf(stderr, "desk: at=%lld link %s (%s)\n", (long long)now,
            link == QLC_READY ? "ready" : link == QLC_FETCHING ? "reading the show"
            : link == QLC_CONNECTING ? "connecting" : "down", app->last_reason);
}
