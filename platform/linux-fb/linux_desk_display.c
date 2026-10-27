#include "linux_desk_display.h"

#include <stdio.h>

#include "display_power.h"

void linux_desk_display(void *ctx, int on) {
    (void)ctx;
    if (!on && display_power_off() != 0)
        fprintf(stderr, "desk: cannot blank the display\n");
    if (on && display_power_on() != 0)
        fprintf(stderr, "desk: cannot wake the display\n");
}
