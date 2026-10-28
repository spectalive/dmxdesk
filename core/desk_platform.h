// What the desk needs from the machine it runs on. desk_run is the desk;
// everything here is a device or a service: where touches come from, where
// a frame goes, the power key, the display, the bar's facts and what the
// settings surface drives. A backend fills one table and passes its own
// state as ctx; core never names a device node, a socket path or a driver.
#ifndef DESK_PLATFORM_H
#define DESK_PLATFORM_H

#include <poll.h>
#include <stdint.h>

#include "desk_setup.h"
#include "status.h"
#include "touch_event.h"

struct desk_app;

struct desk_platform {
    void *ctx;
    // A monotonic clock in milliseconds.
    int64_t (*now)(void *ctx);
    // 1 once the desk was asked to stop from outside: a signal.
    int (*stopping)(void *ctx);
    // Sleeps until a device or one of the master's sockets is ready, at
    // most timeout_ms; the readiness it saw answers the next reads.
    void (*wait)(void *ctx, const struct pollfd *master, int count, int timeout_ms);
    // 1 when the power key was pressed since the last wait.
    int (*power_key)(void *ctx);
    // Up to cap touches in screen pixels, 0 when none, -1 once the source
    // has ended for good.
    int (*touches)(void *ctx, struct touch_event *events, int cap);
    // Forgets every contact the source still tracks; returns how many there were.
    int (*drop_contacts)(void *ctx);
    // Turns the display off (0) or on (1).
    void (*display)(void *ctx, int on);
    // The settings surface opened: the platform may start a scan.
    void (*setup_opened)(void *ctx, struct desk_app *app, int64_t now);
    // What the settings surface asked for that only the machine can do:
    // scan, join, find, the backlight. CLOSE and SET_MASTER are the desk's.
    void (*setup_action)(void *ctx, struct desk_app *app, const struct setup_action *act,
                         int64_t now);
    // Once per turn, before the master is stepped: the services' own progress, which
    // lands on the settings surface.
    void (*service)(void *ctx, struct desk_app *app, int64_t now);
    // The bar's facts, read at most once a second; the platform also keeps
    // the settings surface's brightness and address in step with them.
    void (*status)(void *ctx, struct desk_app *app, struct status *next, int64_t now);
    // The turn's frame: the damaged rectangle (dw < 0 for all of it) when
    // damaged is set. Returns nonzero to end the loop.
    int (*frame)(void *ctx, struct desk_app *app, int damaged, int dx, int dy, int dw, int dh,
                 int64_t now);
};

#endif
