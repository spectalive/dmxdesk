// SOURCES: canvas.c canvas_blend.c desk_action_log.c desk_apply_frame.c desk_apply_speed_echo.c desk_brightness_level_at.c desk_brightness_track.c desk_build_holds.c desk_caption_number.c desk_conf.c desk_damage_running.c desk_damage_speed.c desk_end_gestures.c desk_fake_drag.c desk_family_icon.c desk_follow_page.c desk_hold.c desk_in_gear.c desk_input.c desk_layout_resolve.c desk_link_of.c desk_link_word.c desk_lock.c desk_log_link.c desk_log_rtt.c desk_master_level_at.c desk_master_track.c desk_model.c desk_pager_hit.c desk_pager_label.c desk_paint_keyline.c desk_panel_holds.c desk_pick_numbered.c desk_rebuild_model.c desk_release_holds.c desk_release_hook.c desk_route_touch.c desk_run.c desk_send_action.c desk_send_hold.c desk_send_speed.c desk_send_tagged.c desk_setup.c desk_setup_page_hit.c desk_setup_row_hit.c desk_show_accent_tile.c desk_show_haze.c desk_show_holds.c desk_show_is_rig.c desk_show_layout.c desk_show_live_page.c desk_show_panel.c desk_show_place.c desk_show_rig.c desk_show_section.c desk_show_slot.c desk_speed.c desk_speed_paint.c desk_speed_target_rect.c desk_speed_tempo_step_rect.c desk_status_changed.c desk_sync_link_word.c desk_tab_page.c desk_take_frame_damage.c desk_take_power_key.c desk_take_setup_action.c desk_take_snapshot.c desk_take_touches.c desk_tap.c desk_text_upper.c desk_tick_holds.c desk_tick_session.c desk_tick_status.c desk_toggle_drain.c desk_toggle_queue.c desk_toggle_setup.c desk_touch_content.c desk_touch_gear.c desk_touch_lock.c desk_touch_navigate.c desk_touch_owned.c desk_touch_press.c desk_touch_setup.c desk_touch_speed_card.c desk_trajectory_of.c desk_view.c desk_view_family.c desk_view_pager.c font.c http_fetch.c icon_glyph.c keyboard.c qlc_codec.c qlc_session.c send_queue.c showmap.c showmap_check_priority.c showmap_release_to_parse.c showmap_release_to_prune.c showmap_validate.c speed_factor.c status_wifi_bars.c vcjson.c wifi_scan_joinable.c ws_client.c
// The loop with no Linux under it: a scripted platform feeds touches and
// key presses turn by turn and records what the desk asks of it. The gear
// opens and closes the settings surface, the lock locks, the power key
// blanks and wakes the display, the bar is read once a second, and the loop
// ends on the touch source's end or when the frame says so. No master is
// set, so the session never dials.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "desk_app.h"
#include "desk_build_holds.h"
#include "desk_layout.h"
#include "desk_platform.h"
#include "desk_run.h"
#include "showmap_validate.h"

struct script {
    int64_t clock;
    int turn;
    int end_touches_at;     // the turn whose touches read -1, or -1
    int stop_frame_at;      // the turn whose frame returns 1, or -1
    int key_at[4];          // turns with a power key press, -1 terminated
    struct touch_event touch[16];
    int touch_turn[16];
    int touches;
    int frames;
    int display_calls, display_on;
    int setup_opened;
    int setup_actions;
    int status_reads;
    int damaged_frames;
};

static int64_t fake_now(void *ctx) { return ((struct script *)ctx)->clock; }
static int fake_stopping(void *ctx) { (void)ctx; return 0; }

static void fake_wait(void *ctx, const struct pollfd *session, int count, int timeout_ms) {
    (void)session; (void)count;
    struct script *s = ctx;
    assert(timeout_ms == 100);
    s->clock += 100;
}

static int fake_power_key(void *ctx) {
    struct script *s = ctx;
    for (int i = 0; s->key_at[i] >= 0; i++)
        if (s->key_at[i] == s->turn)
            return 1;
    return 0;
}

static int fake_touches(void *ctx, struct touch_event *events, int cap) {
    struct script *s = ctx;
    if (s->turn == s->end_touches_at)
        return -1;
    int n = 0;
    for (int i = 0; i < s->touches && n < cap; i++)
        if (s->touch_turn[i] == s->turn)
            events[n++] = s->touch[i];
    return n;
}

static int fake_drop_contacts(void *ctx) { (void)ctx; return 0; }

static void fake_display(void *ctx, int on) {
    struct script *s = ctx;
    s->display_calls++;
    s->display_on = on;
}

static void fake_setup_opened(void *ctx, struct desk_app *app, int64_t now) {
    (void)app; (void)now;
    ((struct script *)ctx)->setup_opened++;
}

static void fake_setup_action(void *ctx, struct desk_app *app, const struct setup_action *act,
                              int64_t now) {
    (void)app; (void)act; (void)now;
    ((struct script *)ctx)->setup_actions++;
}

static void fake_service(void *ctx, struct desk_app *app, int64_t now) {
    (void)ctx; (void)app; (void)now;
}

static void fake_status(void *ctx, struct desk_app *app, struct status *next, int64_t now) {
    (void)app; (void)now;
    memset(next, 0, sizeof *next);
    next->have_batt = 1;
    next->cap = 80;
    ((struct script *)ctx)->status_reads++;
}

static int fake_frame(void *ctx, struct desk_app *app, int damaged, int dx, int dy, int dw, int dh,
                      int64_t now) {
    (void)app; (void)dx; (void)dy; (void)dw; (void)dh; (void)now;
    struct script *s = ctx;
    s->frames++;
    s->damaged_frames += damaged != 0;
    return s->turn++ == s->stop_frame_at;
}

static void tap(struct script *s, int turn, int slot, int x, int y) {
    struct touch_event down = { TOUCH_DOWN, slot, (float)x, (float)y, 0 };
    struct touch_event up = { TOUCH_UP, slot, (float)x, (float)y, 0 };
    s->touch[s->touches] = down;
    s->touch_turn[s->touches++] = turn;
    s->touch[s->touches] = up;
    s->touch_turn[s->touches++] = turn + 1;
}

static struct desk_app app;

static void build(void) {
    memset(&app, 0, sizeof app);
    assert(showmap_load("show/vibra.desk.json", &app.map) == 0);
    showmap_build(&app.model, &app.map, &app.console);
    assert(desk_layout_resolve(&app.map, app.map.count, app.map.count + 1, app.map.count + 2,
                               app.map.count + 3, &app.layout) == 0);
    desk_set_layout(&app.model, &app.layout);
    desk_set_view(&app.model, 0, 0);
    desk_input_init(&app.input);
    desk_speed_init(&app.speed, &app.map);
    app.speed_slot = app.setup_slot = app.gear_slot = -1;
    app.last_page = app.model.page;
    desk_build_holds(&app.hold, &app.model, app.hold_owner);
    desk_lock_init(&app.lock);
    desk_setup_init(&app.setup);
    desk_toggle_queue_init(&app.toggles);
    app.last_rtt_logged = -1;
    app.conf_path = "/nonexistent/desk.conf";
    struct qlc_session_config cfg = { .port = 9999, .heartbeat_ms = 250, .stale_ms = 750,
        .reconnect_ms = 1500, .connect_timeout_ms = 3000, .fetch_timeout_ms = 4000,
        .snapshot_limit = 1024 * 1024 };
    app.session = qlc_session_new(&cfg);
    assert(app.session);
}

static struct desk_platform platform_of(struct script *s) {
    struct desk_platform p = {
        .ctx = s, .now = fake_now, .stopping = fake_stopping, .wait = fake_wait,
        .power_key = fake_power_key, .touches = fake_touches,
        .drop_contacts = fake_drop_contacts, .display = fake_display,
        .setup_opened = fake_setup_opened, .setup_action = fake_setup_action,
        .service = fake_service, .status = fake_status, .frame = fake_frame,
    };
    return p;
}

static void script_init(struct script *s) {
    memset(s, 0, sizeof *s);
    s->clock = 10000;
    s->end_touches_at = -1;
    s->stop_frame_at = -1;
    s->key_at[0] = -1;
}

int main(void) {
    struct script s;
    struct desk_platform p;
    int gear_x = DESK_GEAR_X + DESK_TARGET / 2, lock_x = DESK_LOCK_X + DESK_TARGET / 2;

    // The gear opens the surface, which the platform hears about; a second
    // tap closes it. The bar is read on the first turn and once a second.
    build();
    script_init(&s);
    tap(&s, 1, 0, gear_x, 24);
    tap(&s, 4, 0, gear_x, 24);
    s.stop_frame_at = 2;
    p = platform_of(&s);
    desk_run(&app, &p);
    assert(app.setup.open && s.setup_opened == 1);
    assert(s.frames == 3 && s.damaged_frames >= 1);
    assert(app.model.battery == 80);
    s.stop_frame_at = 24;
    desk_run(&app, &p);
    assert(!app.setup.open && s.setup_opened == 1);
    // Turns run 100 ms apart; the surface's open and close each force a read.
    assert(s.status_reads >= 3 && s.status_reads <= 6);

    // A tap on the lock locks the surface and the model draws it locked.
    build();
    script_init(&s);
    tap(&s, 1, 2, lock_x, 24);
    s.stop_frame_at = 4;
    p = platform_of(&s);
    desk_run(&app, &p);
    assert(app.lock.state == DESK_LOCKED && app.model.locked);

    // The power key blanks the display, the next press wakes it locked.
    build();
    script_init(&s);
    s.key_at[0] = 1;
    s.key_at[1] = 3;
    s.key_at[2] = -1;
    s.stop_frame_at = 2;
    p = platform_of(&s);
    desk_run(&app, &p);
    assert(s.display_calls == 1 && s.display_on == 0 && app.lock.state == DESK_BLANKED);
    s.stop_frame_at = 4;
    desk_run(&app, &p);
    assert(s.display_calls == 2 && s.display_on == 1 && app.lock.state == DESK_LOCKED);

    // The touch source's end is the last turn: the loop stops by itself.
    build();
    script_init(&s);
    s.end_touches_at = 3;
    p = platform_of(&s);
    desk_run(&app, &p);
    assert(app.stop && s.frames == 4);

    qlc_session_free(app.session);
    vc_free(&app.console);
    return 0;
}
