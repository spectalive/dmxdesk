#include "desk_tick_status.h"

#include "desk_status_changed.h"
#include "status_wifi_bars.h"

void desk_tick_status(struct desk_app *app, const struct desk_platform *p, int64_t now) {
    if (now - app->last_status_ms < DESK_STATUS_MS)
        return;
    app->last_status_ms = now;
    struct status next;
    p->status(p->ctx, app, &next, now);
    if (desk_status_changed(&app->status, &next) || !app->bar_ready) {
        app->status = next;
        app->bar_ready = 1;
    }
    app->status = next;
    desk_set_status(&app->model, app->status.have_batt ? app->status.cap : -1,
                    app->status.have_batt && app->status.ma > 0,
                    app->status.have_wifi ? status_wifi_bars(&app->status) : 0, app->setup.open);
}
