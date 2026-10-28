// SOURCES: desk_toggle_queue.c
// The spacing queue on its own: order, the 200 ms slot, and the bound.
#include <assert.h>

#include "desk_toggle_queue.h"

static struct desk_command toggle(int id) {
    struct desk_command c = { DESK_CMD_TOGGLE, id, 0 };
    return c;
}

static int is_toggle(const struct desk_command *c, int id) {
    return c->kind == DESK_CMD_TOGGLE && c->id == id;
}

int main(void) {
    struct desk_toggle_queue q;
    desk_toggle_queue_init(&q);
    assert(desk_toggle_queue_count(&q) == 0);

    // Nothing sent yet: the first command's slot is open right away.
    assert(desk_toggle_queue_slot_open(&q, 0));
    struct desk_command out, in;
    assert(desk_toggle_queue_pop(&q, &out) == 0);

    in = toggle(10);
    assert(desk_toggle_queue_push(&q, &in) == 0);
    assert(desk_toggle_queue_count(&q) == 1);
    assert(desk_toggle_queue_slot_open(&q, 1000));
    assert(desk_toggle_queue_pop(&q, &out) == 1 && is_toggle(&out, 10));
    assert(desk_toggle_queue_count(&q) == 0);
    desk_toggle_queue_mark_slot(&q, 1000);

    // A slot used at 1000: not open again until 200 ms later, open at exactly 200.
    assert(!desk_toggle_queue_slot_open(&q, 1000));
    assert(!desk_toggle_queue_slot_open(&q, 1199));
    assert(desk_toggle_queue_slot_open(&q, 1200));

    // FIFO order over three pushes, popped one at a time.
    for (int id = 10; id <= 30; id += 10) {
        in = toggle(id);
        assert(desk_toggle_queue_push(&q, &in) == 0);
    }
    assert(desk_toggle_queue_count(&q) == 3);
    assert(desk_toggle_queue_pop(&q, &out) == 1 && is_toggle(&out, 10));
    assert(desk_toggle_queue_pop(&q, &out) == 1 && is_toggle(&out, 20));
    assert(desk_toggle_queue_pop(&q, &out) == 1 && is_toggle(&out, 30));
    assert(desk_toggle_queue_pop(&q, &out) == 0);

    // Bound: DESK_TOGGLE_QUEUE_CAP commands fit, the next one is refused, and
    // popping still drains exactly the ones that fit, oldest first.
    for (int i = 0; i < DESK_TOGGLE_QUEUE_CAP; i++) {
        in = toggle(i);
        assert(desk_toggle_queue_push(&q, &in) == 0);
    }
    assert(desk_toggle_queue_count(&q) == DESK_TOGGLE_QUEUE_CAP);
    in = toggle(99);
    assert(desk_toggle_queue_push(&q, &in) == -1);
    assert(desk_toggle_queue_count(&q) == DESK_TOGGLE_QUEUE_CAP);
    for (int i = 0; i < DESK_TOGGLE_QUEUE_CAP; i++)
        assert(desk_toggle_queue_pop(&q, &out) == 1 && is_toggle(&out, i));
    assert(desk_toggle_queue_count(&q) == 0);

    // The ring wraps: push/pop past the physical end of the backing array.
    for (int round = 0; round < DESK_TOGGLE_QUEUE_CAP * 2; round++) {
        struct desk_command off = { DESK_CMD_FUNCTION, 40, 0 };
        assert(desk_toggle_queue_push(&q, &off) == 0);
        assert(desk_toggle_queue_pop(&q, &out) == 1 && out.kind == DESK_CMD_FUNCTION &&
               out.id == 40 && out.value == 0);
    }

    return 0;
}
