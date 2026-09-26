// SOURCES: desk_toggle_queue.c
// The spacing queue on its own: order, the 200 ms slot, and the bound.
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "desk_toggle_queue.h"

int main(void) {
    struct desk_toggle_queue q;
    desk_toggle_queue_init(&q);
    assert(desk_toggle_queue_count(&q) == 0);

    // Nothing sent yet: the first frame's slot is open right away.
    assert(desk_toggle_queue_slot_open(&q, 0));
    char out[64];
    assert(desk_toggle_queue_pop(&q, out, sizeof out) == 0);

    assert(desk_toggle_queue_push(&q, "10|255") == 0);
    assert(desk_toggle_queue_count(&q) == 1);
    assert(desk_toggle_queue_slot_open(&q, 1000));
    assert(desk_toggle_queue_pop(&q, out, sizeof out) == 1 && strcmp(out, "10|255") == 0);
    assert(desk_toggle_queue_count(&q) == 0);
    desk_toggle_queue_mark_slot(&q, 1000);

    // A slot used at 1000: not open again until 200 ms later, open at exactly 200.
    assert(!desk_toggle_queue_slot_open(&q, 1000));
    assert(!desk_toggle_queue_slot_open(&q, 1199));
    assert(desk_toggle_queue_slot_open(&q, 1200));

    // FIFO order over three pushes, popped one at a time.
    assert(desk_toggle_queue_push(&q, "10|255") == 0);
    assert(desk_toggle_queue_push(&q, "20|255") == 0);
    assert(desk_toggle_queue_push(&q, "30|255") == 0);
    assert(desk_toggle_queue_count(&q) == 3);
    assert(desk_toggle_queue_pop(&q, out, sizeof out) == 1 && strcmp(out, "10|255") == 0);
    assert(desk_toggle_queue_pop(&q, out, sizeof out) == 1 && strcmp(out, "20|255") == 0);
    assert(desk_toggle_queue_pop(&q, out, sizeof out) == 1 && strcmp(out, "30|255") == 0);
    assert(desk_toggle_queue_pop(&q, out, sizeof out) == 0);

    // Bound: DESK_TOGGLE_QUEUE_CAP frames fit, the next one is refused, and
    // popping still drains exactly the ones that fit, oldest first.
    for (int i = 0; i < DESK_TOGGLE_QUEUE_CAP; i++) {
        char frame[8];
        snprintf(frame, sizeof frame, "%d|255", i);
        assert(desk_toggle_queue_push(&q, frame) == 0);
    }
    assert(desk_toggle_queue_count(&q) == DESK_TOGGLE_QUEUE_CAP);
    assert(desk_toggle_queue_push(&q, "99|255") == -1);
    assert(desk_toggle_queue_count(&q) == DESK_TOGGLE_QUEUE_CAP);
    for (int i = 0; i < DESK_TOGGLE_QUEUE_CAP; i++) {
        char frame[8];
        snprintf(frame, sizeof frame, "%d|255", i);
        assert(desk_toggle_queue_pop(&q, out, sizeof out) == 1 && strcmp(out, frame) == 0);
    }
    assert(desk_toggle_queue_count(&q) == 0);

    // The ring wraps: push/pop past the physical end of the backing array.
    for (int round = 0; round < DESK_TOGGLE_QUEUE_CAP * 2; round++) {
        assert(desk_toggle_queue_push(&q, "40|0") == 0);
        assert(desk_toggle_queue_pop(&q, out, sizeof out) == 1 && strcmp(out, "40|0") == 0);
    }

    return 0;
}
