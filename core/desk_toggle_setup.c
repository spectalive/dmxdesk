#include "desk_toggle_setup.h"

#include "desk_end_gestures.h"

void desk_toggle_setup(struct desk_app *app, const struct desk_platform *p, int64_t now) {
    desk_end_gestures(app, now);
    if (app->setup.open) {
        desk_setup_close(&app->setup);
    } else {
        desk_setup_open(&app->setup);
        p->setup_opened(p->ctx, app, now);
    }
    app->bar_ready = 0;
    app->last_status_ms = 0;
    desk_set_status(&app->model, app->model.battery, app->model.charging, app->model.wifi_bars,
                    app->setup.open);
}
