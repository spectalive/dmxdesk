// SOURCES: desk_pipe_parse.c
#include <assert.h>
#include <math.h>

#include "desk_pipe_parse.h"

int main(void) {
    struct touch_event event;
    assert(desk_pipe_parse("D 2 123 456 987654", &event) == 1);
    assert(event.kind == TOUCH_DOWN && event.slot == 2);
    assert(event.x == 123 && event.y == 456);
    assert(fabs(event.t - 987.654) < 0.0001);
    assert(desk_pipe_parse("M 2 124 457 987700", &event) == 1);
    assert(event.kind == TOUCH_MOVE);
    assert(desk_pipe_parse("U 2 124 457 987800", &event) == 1);
    assert(event.kind == TOUCH_UP);
    assert(desk_pipe_parse("C 2 0 0 987900", &event) == 1);
    assert(event.kind == TOUCH_CANCEL);
    assert(desk_pipe_parse("D 10 1 1 1", &event) == 0);
    assert(desk_pipe_parse("D -1 1 1 1", &event) == 0);
    assert(desk_pipe_parse("D 0 1 1 -1", &event) == 0);
    assert(desk_pipe_parse("D 0 1 1 1 extra", &event) == 0);
    assert(desk_pipe_parse("D 0 1", &event) == 0);
    assert(desk_pipe_parse("X 0 1 1 1", &event) == 0);
    assert(desk_pipe_parse("", &event) == 0);
    return 0;
}
