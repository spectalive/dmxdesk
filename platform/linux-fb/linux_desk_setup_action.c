#include "linux_desk_setup_action.h"

#include <stdio.h>

#include "desk_finder_start.h"
#include "linux_desk.h"
#include "linux_desk_queue_scan.h"

void linux_desk_setup_action(void *ctx, struct desk_app *app, const struct setup_action *act,
                             int64_t now) {
    struct linux_desk *d = ctx;
    struct desk_setup *setup = &app->setup;
    switch (act->kind) {
    case SETUP_SCAN:
        if (d->wpa)
            linux_desk_queue_scan(ctx, app, now);
        else
            snprintf(setup->wifi_note, sizeof setup->wifi_note, "Scan refused");
        break;
    case SETUP_JOIN:
        if (!d->wpa) {
            setup->wifi_busy[0] = '\0';
            break;
        }
        if (wifi_join_start(&d->join, act->ssid, act->psk[0] ? act->psk : NULL,
                            act->known, setup->ssid, now) != 0) {
            setup->wifi_busy[0] = '\0';
            snprintf(setup->wifi_note, sizeof setup->wifi_note, "%.*s",
                     (int)sizeof setup->wifi_note - 1, d->join.reason);
        } else {
            snprintf(setup->wifi_busy, sizeof setup->wifi_busy, "%s", d->join.word);
        }
        fprintf(stderr, "desk: join %s: %s\n", act->ssid,
                d->join.running ? "started" : d->join.reason);
        break;
    case SETUP_FIND:
        setup->master_note[0] = '\0';
        desk_setup_set_found(setup, NULL, 0, 0);
        if (!app->status.have_wifi || !app->status.addr[0]) {
            snprintf(setup->master_note, sizeof setup->master_note, "No network address");
            fprintf(stderr, "desk: find: no address on wlan0\n");
        } else if (desk_finder_start(d->finder, d->exe, app->status.addr, setup->port) == 0) {
            snprintf(setup->master_busy, sizeof setup->master_busy, "Finding");
        } else {
            snprintf(setup->master_note, sizeof setup->master_note, "Could not sweep");
            fprintf(stderr, "desk: find: cannot start the sweep\n");
        }
        break;
    case SETUP_FIND_CANCEL:
        // The sweep dies now; its poll reports it stopped and the card says so.
        aw_cancel(d->finder);
        snprintf(setup->master_note, sizeof setup->master_note, "Search stopped");
        break;
    case SETUP_BRIGHTNESS:
        desk_power_set_level(&d->power, act->value, now);
        break;
    case SETUP_POWER_AWARE:
        desk_power_set_aware(&d->power, act->value, now);
        break;
    default:
        break;
    }
}
