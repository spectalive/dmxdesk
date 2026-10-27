#include "desk_apply_speed_echo.h"

#include <stdio.h>

void desk_apply_speed_echo(struct desk_speed *speed, int widget_id, int ms, int factor, int64_t now) {
    desk_speed_apply(speed, widget_id, ms, factor, now);
    fprintf(stderr, "speed: echo %d|SPEED_STATE|%d|%d at %lld\n", widget_id, ms, factor, (long long)now);
}
