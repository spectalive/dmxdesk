// SOURCES: desk_send_action.c desk_send_command.c desk_toggle_drain.c desk_toggle_queue.c desk_action_log.c qlc_codec.c qlc_encode_command.c desk_link_of.c desk_link_word.c
// What one gesture sends, through a fake console that records every
// command: toggles wait in the queue, stop-all empties it and goes at once,
// haze-off pushes out the oldest waiting toggle, the master goes straight
// out, and a toggle whose slot comes while the link is down is dropped, not
// resent.
#include <assert.h>
#include <string.h>

#include "desk_link_of.h"
#include "desk_link_word.h"
#include "desk_send_action.h"
#include "desk_toggle_drain.h"
#include "qlc_encode_command.h"

static struct desk_command sent[16];
static int sent_count;
static int link_up = 1;
static int hook_widget = -1;
static int noted;

static int fake_send(void *ctx, const struct desk_command *cmd) {
    (void)ctx;
    if (!link_up)
        return -1;
    assert(sent_count < 16);
    sent[sent_count++] = *cmd;
    return 0;
}

static int fake_describe(void *ctx, const struct desk_command *cmd, char *buf, size_t cap) {
    (void)ctx;
    return qlc_encode_command(buf, cap, cmd);
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

static int is(const struct desk_command *c, enum desk_command_kind kind, int id, int value) {
    return c->kind == kind && c->id == id && c->value == value;
}

static void reset(struct desk_toggle_queue *q) {
    desk_toggle_queue_init(q);
    sent_count = 0;
    link_up = 1;
    hook_widget = -1;
    noted = 0;
}

int main(void) {
    struct desk_console console = { .send = fake_send, .describe = fake_describe };
    const struct desk_console *c = &console;
    struct desk_toggle_queue q;
    struct desk_command cmd, last;

    // A toggle is queued, not sent, and marked pending once.
    reset(&q);
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 10, 0), 1000);
    assert(sent_count == 0 && desk_toggle_queue_count(&q) == 1 && noted == 1);

    // A release with a hook queues the hook behind it, both pending.
    reset(&q);
    hook_widget = 20;
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 10, 0), 1000);
    assert(desk_toggle_queue_count(&q) == 2 && noted == 2);
    assert(desk_toggle_queue_pop(&q, &cmd) && is(&cmd, DESK_CMD_TOGGLE, 10, 0));
    assert(desk_toggle_queue_pop(&q, &cmd) && is(&cmd, DESK_CMD_TOGGLE, 20, 0));

    // A full queue drops the toggle and does not mark it pending.
    reset(&q);
    for (int i = 0; i < DESK_TOGGLE_QUEUE_CAP; i++)
        desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 100 + i, 0), 1000);
    noted = 0;
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 200, 0), 1000);
    assert(desk_toggle_queue_count(&q) == DESK_TOGGLE_QUEUE_CAP && noted == 0);

    // Haze-off always gets in: the oldest waiting toggle makes room.
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_HAZE_OFF, 7, 0), 1000);
    assert(desk_toggle_queue_count(&q) == DESK_TOGGLE_QUEUE_CAP && sent_count == 0);
    assert(desk_toggle_queue_pop(&q, &cmd) && is(&cmd, DESK_CMD_TOGGLE, 101, 0));
    while (desk_toggle_queue_pop(&q, &cmd))
        last = cmd;
    assert(is(&last, DESK_CMD_FUNCTION, 7, 0));

    // Stop-all empties the queue first, then goes out at once.
    reset(&q);
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 10, 0), 1000);
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 11, 0), 1000);
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_STOP_ALL, 5, 0), 1000);
    assert(desk_toggle_queue_count(&q) == 0 && sent_count == 1 &&
           is(&sent[0], DESK_CMD_STOP_ALL, 5, 0));

    // The master is sent straight away.
    reset(&q);
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_MASTER, 0, 128), 1000);
    assert(sent_count == 1 && is(&sent[0], DESK_CMD_GRAND_MASTER, 0, 128));

    // A toggle's slot: sent when the link is up; dropped, not kept, when down.
    reset(&q);
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 10, 0), 1000);
    desk_send_action(c, &q, NULL, NULL, act(DESK_ACT_TOGGLE, 11, 0), 1000);
    desk_toggle_drain(c, &q, 1000);
    assert(sent_count == 1 && is(&sent[0], DESK_CMD_TOGGLE, 10, 0) &&
           desk_toggle_queue_count(&q) == 1);
    desk_toggle_drain(c, &q, 1000 + DESK_TOGGLE_MIN_GAP_MS - 1);
    assert(sent_count == 1);
    link_up = 0;
    desk_toggle_drain(c, &q, 1000 + DESK_TOGGLE_MIN_GAP_MS);
    assert(sent_count == 1 && desk_toggle_queue_count(&q) == 0);

    // The banner: down only without a master to retry.
    assert(desk_link_of(DESK_CONSOLE_READY, 1) == DESK_LINK_READY);
    assert(desk_link_of(DESK_CONSOLE_FETCHING, 1) == DESK_LINK_SYNCING);
    assert(desk_link_of(DESK_CONSOLE_CONNECTING, 0) == DESK_LINK_CONNECTING);
    assert(desk_link_of(DESK_CONSOLE_DOWN, 1) == DESK_LINK_CONNECTING);
    assert(desk_link_of(DESK_CONSOLE_DOWN, 0) == DESK_LINK_DOWN);
    assert(strcmp(desk_link_word(DESK_CONSOLE_READY), "Linked") == 0);
    assert(strcmp(desk_link_word(DESK_CONSOLE_DOWN), "Not linked") == 0);
    return 0;
}
