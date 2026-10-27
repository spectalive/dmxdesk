// Every ten seconds, how many frames were flipped and, with DMXDESK_PERF,
// the paint and present percentiles with RSS and MemAvailable; on SIGUSR1,
// the canvas to /tmp/desk.ppm without stopping.
#ifndef LINUX_DESK_REPORT_H
#define LINUX_DESK_REPORT_H

#include <stdint.h>

#include "desk_app.h"
#include "linux_desk.h"

void linux_desk_report(struct linux_desk *d, struct desk_app *app, int64_t now);

#endif
