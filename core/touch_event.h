// A contact as the desk sees it: down, moved, lifted or cancelled, in panel
// coordinates. The platform's touch reader produces these; the desk consumes
// them without knowing where they came from.
#ifndef TOUCH_EVENT_H
#define TOUCH_EVENT_H

enum touch_kind {
    TOUCH_DOWN,
    TOUCH_MOVE,
    TOUCH_UP,
    TOUCH_CANCEL,
};

struct touch_event {
    enum touch_kind kind;
    int slot;
    float x;
    float y;
    double t;
};

#define TOUCH_MAX_SLOTS 10

#endif
