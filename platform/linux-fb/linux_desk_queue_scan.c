#include "linux_desk_queue_scan.h"

#include <stdio.h>

#include "linux_desk.h"

void linux_desk_queue_scan(void *ctx, struct desk_app *app, int64_t now) {
    struct linux_desk *d = ctx;
    d->wifi_request.scan_queued = 1;
    snprintf(app->setup.wifi_busy, sizeof app->setup.wifi_busy, "Scanning");
    d->scan_started_ms = now;
}
