#include "desk_log_link.h"

#include <stdio.h>
#include <string.h>

void desk_log_link(struct desk_app *app, int64_t now) {
    enum desk_console_link link = app->link;
    if (link == app->last_link && strcmp(app->last_reason, app->master.reason(app->master.ctx)) == 0)
        return;
    app->last_link = link;
    snprintf(app->last_reason, sizeof app->last_reason, "%s", app->master.reason(app->master.ctx));
    fprintf(stderr, "desk: at=%lld link %s (%s)\n", (long long)now,
            link == DESK_CONSOLE_READY ? "ready" : link == DESK_CONSOLE_FETCHING ? "reading the show"
            : link == DESK_CONSOLE_CONNECTING ? "connecting" : "down", app->last_reason);
}
