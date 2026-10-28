// Everything the desk knows between two turns of its loop: the show, the
// console it was checked against, the models behind the surface, who owns
// which finger, the settings surface and the console it drives. One
// instance per desk; desk_run turns it, a platform fills its devices in.
#ifndef DESK_APP_H
#define DESK_APP_H

#include <stdint.h>

#include "canvas.h"
#include "desk_conf.h"
#include "desk_console.h"
#include "desk_fonts.h"
#include "desk_hold.h"
#include "desk_input.h"
#include "desk_layout_resolve.h"
#include "desk_lock.h"
#include "desk_model.h"
#include "desk_setup.h"
#include "desk_speed.h"
#include "desk_toggle_queue.h"
#include "showmap.h"
#include "status.h"
#include "touch_event.h"
#include "vcjson.h"

struct desk_app {
    struct show_map map;
    struct desk_layout layout;
    struct desk_model model;
    struct vc_doc console;
    struct desk_input input;
    struct desk_speed speed;
    struct desk_hold hold;
    int hold_owner[TOUCH_MAX_SLOTS];    // the control each finger holds, or -1
    struct desk_lock lock;
    struct desk_setup setup;
    struct desk_toggle_queue toggles;
    struct desk_console master;         // the lighting console the desk drives
    struct desk_conf conf;
    const char *conf_path;              // where SET_MASTER saves the master
    struct desk_fonts fonts;
    struct canvas canvas;               // no pixels when nothing is painted here
    struct status status;

    int speed_slot;     // the finger the speed cards own, if any
    int setup_slot;     // the one finger the settings surface owns
    int gear_slot;
    int last_page;
    int bar_ready;      // the bar's facts reach the model on the next status tick
    int64_t last_status_ms;
    enum desk_console_link link;
    enum desk_console_link last_link;
    char last_reason[96];
    int last_rtt_logged;

    int log_rtt;        // DMXDESK_RTT: print every heartbeat's round trip
    int fake_drag;      // DMXDESK_DRAG: drag the master every loop, sending nothing
    int no_wait;        // the loop never sleeps: forced flips run at the panel's pace
    int stop;           // the touch source ended: this turn is the last
};

#endif
