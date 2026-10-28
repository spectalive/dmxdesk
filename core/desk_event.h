// What a lighting console told the desk, decoded by its adapter. Anything
// the desk does not act on arrives as DESK_EVENT_NONE.
#ifndef DESK_EVENT_H
#define DESK_EVENT_H

enum desk_event_kind {
    DESK_EVENT_NONE,
    DESK_EVENT_FUNCTION,        // id: a function; running: 1 or 0
    DESK_EVENT_GRAND_MASTER,    // value: 0-255
    DESK_EVENT_SPEED,           // id: a speed dial; value: ms; factor: 0..10
};

struct desk_event {
    enum desk_event_kind kind;
    int id;
    int value;
    int factor;
    int running;
};

#endif
