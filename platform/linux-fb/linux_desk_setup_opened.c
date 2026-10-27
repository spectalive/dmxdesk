#include "linux_desk_setup_opened.h"

#include "linux_desk.h"
#include "linux_desk_queue_scan.h"

void linux_desk_setup_opened(void *ctx, struct desk_app *app, int64_t now) {
    struct linux_desk *d = ctx;
    if (d->wpa && !app->setup.wifi_busy[0] && !d->join.running)
        linux_desk_queue_scan(ctx, app, now);
}
