// Spaces toggle commands at least DESK_TOGGLE_MIN_GAP_MS apart, in order: a
// toggle struck less than one gap after the last one's slot waits for its
// own slot instead of landing in the same engine tick (2026-09-26 - AUTO and
// a colour pick of the same solo frame in one tick left the pick's colour
// stopped with RGB at 0 while AUTO ran; >=200 ms between presses did not
// reproduce it). Bursts and the master fader are not queued, only toggles.
#ifndef DESK_TOGGLE_QUEUE_H
#define DESK_TOGGLE_QUEUE_H

#include <stdint.h>

#include "desk_command.h"

#define DESK_TOGGLE_QUEUE_CAP 8
#define DESK_TOGGLE_MIN_GAP_MS 200

struct desk_toggle_queue {
    struct desk_command command[DESK_TOGGLE_QUEUE_CAP];
    int head;
    int count;
    int64_t last_slot_ms;
    int has_slot;    // no slot spent yet: the first command's slot is open at once
};

void desk_toggle_queue_init(struct desk_toggle_queue *q);
int desk_toggle_queue_count(const struct desk_toggle_queue *q);
// Queues a copy of command at the back. Returns 0, or -1 when the queue
// already holds DESK_TOGGLE_QUEUE_CAP commands: the caller drops and logs it,
// nothing queued is ever overwritten or reordered.
int desk_toggle_queue_push(struct desk_toggle_queue *q, const struct desk_command *command);
// True once DESK_TOGGLE_MIN_GAP_MS has passed since the last slot spent, or
// none has been spent yet.
int desk_toggle_queue_slot_open(const struct desk_toggle_queue *q, int64_t now);
// Pops the oldest queued command into out and returns 1, or returns 0
// when the queue is empty. Does not check the slot itself; the
// caller checks desk_toggle_queue_slot_open first.
int desk_toggle_queue_pop(struct desk_toggle_queue *q, struct desk_command *out);
// Records now as the slot just spent, whether its command was sent or dropped:
// a spent slot keeps the next command's spacing correct either way.
void desk_toggle_queue_mark_slot(struct desk_toggle_queue *q, int64_t now);

#endif
