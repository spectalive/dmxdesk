#include "linux_desk_present.h"

#include "desk_dump_ppm.h"
#include "desk_paint_frame.h"
#include "linux_desk_now.h"

int linux_desk_present(struct linux_desk *d, struct desk_app *app, int damaged,
                       int dx, int dy, int dw, int dh, int64_t now) {
    if (!damaged && !d->force_flip)
        return 0;
    if (damaged) {
        int64_t t0 = linux_desk_now(d);
        desk_paint_frame(app, dx, dy, dw, dh);
        if (d->log_perf)
            perf_window_add(&d->paint_ms, (int)(linux_desk_now(d) - t0));
    }
    int64_t t1 = linux_desk_now(d);
    if (present_frame_damage(d->present, &app->canvas, dx, dy, dw, dh) != 0)
        return 1;
    if (d->log_perf)
        perf_window_add(&d->present_ms, (int)(linux_desk_now(d) - t1));
    d->flips++;
    if (d->dump && (app->link == DESK_CONSOLE_READY || now - d->start_ms > 6000)) {
        desk_dump_ppm(&app->canvas, d->dump);
        return 1;
    }
    return 0;
}
