// SOURCES: desk_send_action.c desk_send_tagged.c desk_toggle_drain.c desk_toggle_queue.c desk_action_log.c qlc_codec.c desk_link_of.c desk_link_word.c
// What one gesture sends, through a fake session that records every frame:
// toggles wait in the queue, stop-all empties it and goes at once, haze-off
// pushes out the oldest waiting toggle, the master goes straight out, and a
// toggle whose slot comes while the link is down is dropped, not resent.
#include <assert.h>
#include <string.h>

#include "desk_link_of.h"
#include "desk_link_word.h"
#include "desk_send_action.h"
#include "desk_toggle_drain.h"
#include "qlc_codec.h"

static char sent[16][64];
static int sent_count;
static int link_up = 1;
static int hook_widget = -1;
static int noted;

int qlc_session_send(struct qlc_session *s, const char *frame) {
    (void)s;
    if (!link_up)
        return -1;
    assert(sent_count < 16);
    strcpy(sent[sent_count++], frame);
    return 0;
}

int desk_release_hook(const struct desk_model *model, const struct show_map *map, int widget_id) {
    (void)model; (void)map; (void)widget_id;
    return hook_widget;
}

void desk_note_sent(struct desk_model *model, int widget_id, int64_t now) {
    (void)model; (void)widget_id; (void)now;
    noted++;
}

static struct desk_action act(enum desk_action_kind kind, int widget_id, int value) {
    struct desk_action a = { kind, widget_id, value };
    return a;
}

static void reset(struct desk_toggle_queue *q) {
    desk_toggle_queue_init(q);
    sent_count = 0;
    link_up = 1;
    hook_widget = -1;
    noted = 0;
}

int main(void) {
    struct desk_toggle_queue q;
    char frame[64], expect[64];

    // A toggle is queued, not sent, and marked pending once.
    reset(&q);
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 10, 0), 1000);
    assert(sent_count == 0 && desk_toggle_queue_count(&q) == 1 && noted == 1);

    // A release with a hook queues the hook behind it, both pending.
    reset(&q);
    hook_widget = 20;
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 10, 0), 1000);
    assert(desk_toggle_queue_count(&q) == 2 && noted == 2);
    qlc_encode_toggle(expect, sizeof expect, 10);
    assert(desk_toggle_queue_pop(&q, frame, sizeof frame) && strcmp(frame, expect) == 0);
    qlc_encode_toggle(expect, sizeof expect, 20);
    assert(desk_toggle_queue_pop(&q, frame, sizeof frame) && strcmp(frame, expect) == 0);

    // A full queue drops the toggle and does not mark it pending.
    reset(&q);
    for (int i = 0; i < DESK_TOGGLE_QUEUE_CAP; i++)
        desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 100 + i, 0), 1000);
    noted = 0;
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 200, 0), 1000);
    assert(desk_toggle_queue_count(&q) == DESK_TOGGLE_QUEUE_CAP && noted == 0);

    // Haze-off always gets in: the oldest waiting toggle makes room.
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_HAZE_OFF, 7, 0), 1000);
    assert(desk_toggle_queue_count(&q) == DESK_TOGGLE_QUEUE_CAP && sent_count == 0);
    qlc_encode_toggle(expect, sizeof expect, 101);
    assert(desk_toggle_queue_pop(&q, frame, sizeof frame) && strcmp(frame, expect) == 0);
    char last[64] = "";
    while (desk_toggle_queue_pop(&q, frame, sizeof frame))
        strcpy(last, frame);
    qlc_encode_function_status(expect, sizeof expect, 7, 0);
    assert(strcmp(last, expect) == 0);

    // Stop-all empties the queue first, then goes out at once.
    reset(&q);
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 10, 0), 1000);
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 11, 0), 1000);
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_STOP_ALL, 5, 0), 1000);
    qlc_encode_stop_all(expect, sizeof expect, 5);
    assert(desk_toggle_queue_count(&q) == 0 && sent_count == 1 && strcmp(sent[0], expect) == 0);

    // The master is sent straight away.
    reset(&q);
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_MASTER, 0, 128), 1000);
    qlc_encode_grand_master(expect, sizeof expect, 128);
    assert(sent_count == 1 && strcmp(sent[0], expect) == 0);

    // A toggle's slot: sent when the link is up; dropped, not kept, when down.
    reset(&q);
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 10, 0), 1000);
    desk_send_action(NULL, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 11, 0), 1000);
    desk_toggle_drain(NULL, &q, 1000);
    assert(sent_count == 1 && desk_toggle_queue_count(&q) == 1);
    desk_toggle_drain(NULL, &q, 1000 + DESK_TOGGLE_MIN_GAP_MS - 1);
    assert(sent_count == 1);
    link_up = 0;
    desk_toggle_drain(NULL, &q, 1000 + DESK_TOGGLE_MIN_GAP_MS);
    assert(sent_count == 1 && desk_toggle_queue_count(&q) == 0);

    // The banner: down only without a master to retry.
    assert(desk_link_of(QLC_READY, 1) == DESK_LINK_READY);
    assert(desk_link_of(QLC_FETCHING, 1) == DESK_LINK_SYNCING);
    assert(desk_link_of(QLC_CONNECTING, 0) == DESK_LINK_CONNECTING);
    assert(desk_link_of(QLC_DOWN, 1) == DESK_LINK_CONNECTING);
    assert(desk_link_of(QLC_DOWN, 0) == DESK_LINK_DOWN);
    assert(strcmp(desk_link_word(QLC_READY), "Linked") == 0);
    assert(strcmp(desk_link_word(QLC_DOWN), "Not linked") == 0);
    return 0;
}
