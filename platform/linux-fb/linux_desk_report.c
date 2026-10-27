#include "linux_desk_report.h"

#include <stdio.h>

#include "desk_dump_ppm.h"

void linux_desk_report(struct linux_desk *d, struct desk_app *app, int64_t now) {
    if (now - d->last_report_ms >= 10000) {
        d->last_report_ms = now;
        printf("desk: %lu flips so far, link %s (%s)\n", d->flips,
               app->link == QLC_READY ? "up" : "down", qlc_session_reason(app->session));
        if (d->log_perf) {
            printf("perf: paint p50 %d p95 %d ms (%d) present p50 %d p95 %d ms (%d)"
                   " rss %ld kB memavail %ld kB\n",
                   perf_window_percentile(&d->paint_ms, 50), perf_window_percentile(&d->paint_ms, 95),
                   d->paint_ms.count, perf_window_percentile(&d->present_ms, 50),
                   perf_window_percentile(&d->present_ms, 95), d->present_ms.count,
                   perf_rss_kb(), perf_memavailable_kb());
        }
        fflush(stdout);
    }
    if (*d->snapshot) {
        *d->snapshot = 0;
        desk_dump_ppm(&app->canvas, "/tmp/desk.ppm");
        printf("desk: wrote /tmp/desk.ppm\n");
        fflush(stdout);
    }
}
