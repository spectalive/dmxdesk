// The Linux backend's state behind desk_platform: evdev touch or the Qt
// pipe, the power key, DRM/KMS or @STATE lines out, the backlight, the
// supplicant and the subnet sweep. One per process; main fills it.
#ifndef LINUX_DESK_H
#define LINUX_DESK_H

#include <signal.h>
#include <stdint.h>

#include "action_worker.h"
#include "desk_pipe_read.h"
#include "desk_power.h"
#include "desk_present_drm.h"
#include "desk_wifi_request.h"
#include "perf_window.h"
#include "power_key.h"
#include "touch_input.h"
#include "wifi_join.h"
#include "wpa_ctrl.h"

struct linux_desk {
    volatile sig_atomic_t *stop;        // SIGTERM/SIGINT
    volatile sig_atomic_t *snapshot;    // SIGUSR1: write /tmp/desk.ppm
    int qt_pipe;                        // touches on stdin, @STATE lines on stdout
    struct desk_pipe_reader pipe_reader;
    int touch_fd;
    struct touch_input *touch;
    int touch_ready;                    // the last wait saw the touch node readable
    int power_fd;
    struct power_key power_key;
    int power_ready;
    struct wpa_ctrl *wpa;
    struct wifi_join join;
    struct desk_wifi_request wifi_request;
    int64_t scan_started_ms;
    int scan_dispatched;    // the queued SCAN has actually gone to the daemon
    struct action_worker *finder;
    char exe[256];          // this binary, run again as the sweep
    struct desk_power power;
    struct present *present;
    const char *dump;       // DMXDESK_DUMP: the first linked frame's file
    int force_flip;         // DMXDESK_FLIP: flip every turn
    int log_perf;           // DMXDESK_PERF: paint and present percentiles
    struct perf_window paint_ms, present_ms;
    unsigned long flips;
    int64_t start_ms;
    int64_t last_report_ms;
    int64_t last_snapshot_ms;
    int snapshot_pending;
};

#endif
