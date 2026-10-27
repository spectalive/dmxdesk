#include "desk_toggle_queue.h"

#include <stdio.h>

void desk_toggle_queue_init(struct desk_toggle_queue *q) {
    q->head = 0;
    q->count = 0;
    q->last_slot_ms = 0;
    q->has_slot = 0;
}

int desk_toggle_queue_count(const struct desk_toggle_queue *q) { return q->count; }

int desk_toggle_queue_push(struct desk_toggle_queue *q, const char *frame) {
    if (q->count >= DESK_TOGGLE_QUEUE_CAP)
        return -1;
    int tail = (q->head + q->count) % DESK_TOGGLE_QUEUE_CAP;
    snprintf(q->frame[tail], DESK_TOGGLE_FRAME_MAX, "%s", frame);
    q->count++;
    return 0;
}

int desk_toggle_queue_slot_open(const struct desk_toggle_queue *q, int64_t now) {
    return !q->has_slot || now - q->last_slot_ms >= DESK_TOGGLE_MIN_GAP_MS;
}

int desk_toggle_queue_pop(struct desk_toggle_queue *q, char *out, size_t cap) {
    if (q->count == 0)
        return 0;
    snprintf(out, cap, "%s", q->frame[q->head]);
    q->head = (q->head + 1) % DESK_TOGGLE_QUEUE_CAP;
    q->count--;
    return 1;
}

void desk_toggle_queue_mark_slot(struct desk_toggle_queue *q, int64_t now) {
    q->last_slot_ms = now;
    q->has_slot = 1;
}
