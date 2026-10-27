#include "desk_pipe_parse.h"

#include <stdio.h>

int desk_pipe_parse(const char *line, struct touch_event *event) {
    if (!line || !event)
        return 0;
    char kind, trailing;
    int slot, x, y;
    long long at_ms;
    if (sscanf(line, " %c %d %d %d %lld %c", &kind, &slot, &x, &y, &at_ms,
               &trailing) != 5)
        return 0;
    if (slot < 0 || slot >= TOUCH_MAX_SLOTS || x < -10000 || x > 10000 ||
        y < -10000 || y > 10000 || at_ms < 0)
        return 0;
    switch (kind) {
    case 'D': event->kind = TOUCH_DOWN; break;
    case 'M': event->kind = TOUCH_MOVE; break;
    case 'U': event->kind = TOUCH_UP; break;
    case 'C': event->kind = TOUCH_CANCEL; break;
    default: return 0;
    }
    event->slot = slot;
    event->x = (float)x;
    event->y = (float)y;
    event->t = at_ms / 1000.0;
    return 1;
}
