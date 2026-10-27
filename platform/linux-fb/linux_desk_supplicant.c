#include "linux_desk_supplicant.h"

#include <stdio.h>
#include <string.h>

#include "desk_mark_known.h"
#include "wifi_scan.h"
#include "wpa_status_field.h"

#define SCAN_TIMEOUT_MS 15000

void linux_desk_supplicant(struct linux_desk *d, struct desk_app *app, int64_t now) {
    struct desk_setup *setup = &app->setup;
    struct wifi_join *join = &d->join;
    char ev[512];
    while (wpa_ctrl_event(d->wpa, ev, sizeof ev) == 1) {
        if (strstr(ev, "CTRL-EVENT-SCAN-RESULTS"))
            d->wifi_request.results_queued = 1;
        if (join->running && d->wifi_request.pending == DESK_WIFI_NONE)
            wifi_join_step(join, now, ev, app->status.addr);
    }
    static char reply[16384];
    enum desk_wifi_command done = desk_wifi_request_step(&d->wifi_request, d->wpa,
        now, join->running, setup->open, reply, sizeof reply);
    // FAIL-BUSY means a scan is already under way: its results will come on
    // the same event, so it is not a refusal.
    if (done == DESK_WIFI_SCAN && (d->wifi_request.result < 0 ||
        (strncmp(reply, "OK", 2) != 0 && strncmp(reply, "FAIL-BUSY", 9) != 0))) {
        if (!join->running)
            setup->wifi_busy[0] = '\0';
        snprintf(setup->wifi_note, sizeof setup->wifi_note, "Scan refused");
        fprintf(stderr, "wifi: SCAN answered %d: %.40s\n", d->wifi_request.result, reply);
        setup->dirty = 1;
    } else if (done == DESK_WIFI_RESULTS) {
        if (d->wifi_request.result > 0 && strncmp(reply, "FAIL", 4) != 0) {
            struct wifi_scan scan;
            wifi_scan_parse(reply, strlen(reply), &scan);
            desk_mark_known(setup, &scan);
        } else {
            snprintf(setup->wifi_note, sizeof setup->wifi_note, "Scan results unavailable");
        }
        if (strcmp(setup->wifi_busy, "Scanning") == 0)
            setup->wifi_busy[0] = '\0';
        setup->dirty = 1;
    } else if (done == DESK_WIFI_STATUS && d->wifi_request.result > 0 && setup->open) {
        char ssid[WIFI_SSID_MAX], state[SETUP_WORD_MAX];
        wpa_status_field(reply, "ssid", ssid, sizeof ssid);
        wpa_status_field(reply, "wpa_state", state, sizeof state);
        if (strcmp(ssid, setup->ssid) != 0 || strcmp(state, setup->wifi_state) != 0 ||
            strcmp(app->status.addr, setup->address) != 0)
            desk_setup_set_wifi(setup, ssid, state, app->status.addr);
    }
    if (join->running && d->wifi_request.pending == DESK_WIFI_NONE)
        wifi_join_step(join, now, NULL, app->status.addr);
    if (join->running) {
        if (strcmp(setup->wifi_busy, join->word) != 0) {
            snprintf(setup->wifi_busy, sizeof setup->wifi_busy, "%s", join->word);
            setup->dirty = 1;
        }
    } else if (join->state == WIFI_JOIN_DONE || join->state == WIFI_JOIN_FAILED) {
        if (join->state == WIFI_JOIN_DONE)
            snprintf(setup->wifi_note, sizeof setup->wifi_note, "Joined %s", join->ssid);
        else
            snprintf(setup->wifi_note, sizeof setup->wifi_note, "%.*s",
                     (int)sizeof setup->wifi_note - 1, join->reason);
        fprintf(stderr, "desk: join %s: %s\n", join->ssid, setup->wifi_note);
        join->state = WIFI_JOIN_IDLE;
        setup->wifi_busy[0] = '\0';
        setup->dirty = 1;
        desk_mark_known(setup, &setup->scan);
    }
    if (d->wifi_request.pending == DESK_WIFI_SCAN && !d->scan_dispatched) {
        d->scan_dispatched = 1;
        d->scan_started_ms = now;
        if (!join->running) {
            snprintf(setup->wifi_busy, sizeof setup->wifi_busy, "Scanning");
            setup->dirty = 1;
        }
    } else if (d->wifi_request.pending != DESK_WIFI_SCAN && !d->wifi_request.scan_queued) {
        d->scan_dispatched = 0;
    }
    if (setup->wifi_busy[0] && strcmp(setup->wifi_busy, "Scanning") == 0 &&
        d->scan_dispatched && now - d->scan_started_ms > SCAN_TIMEOUT_MS) {
        setup->wifi_busy[0] = '\0';
        snprintf(setup->wifi_note, sizeof setup->wifi_note, "Scan timed out");
        setup->dirty = 1;
    }
}
