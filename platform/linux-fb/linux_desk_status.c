#include "linux_desk_status.h"

#include <string.h>

#include "linux_desk.h"
#include "linux_desk_paths.h"
#include "status_read.h"
#include "wifi_status.h"

void linux_desk_status(void *ctx, struct desk_app *app, struct status *next, int64_t now) {
    struct linux_desk *d = ctx;
    struct desk_setup *setup = &app->setup;
    status_read(next);
    desk_power_tick(&d->power, next, now);
    if (!setup->open)
        return;
    if (d->power.level != setup->brightness && !setup->dragging_fader) {
        setup->brightness = d->power.level;
        setup->dirty = 1;
    }
    if (setup->brightness_unsaved != d->power.save_failed) {
        setup->brightness_unsaved = d->power.save_failed;
        setup->dirty = 1;
    }
    if (!d->wpa && strcmp(next->addr, setup->address) != 0) {
        char ssid[WIFI_SSID_MAX] = "";
        wifi_read_ssid(WIFI_CONF_PATH, ssid, sizeof ssid);
        desk_setup_set_wifi(setup, ssid, next->have_wifi ? "COMPLETED" : "", next->addr);
    }
}
