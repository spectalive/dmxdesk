#include "osc_console_reason.h"

#include "osc_link.h"

const char *osc_console_reason(void *ctx) {
    return ((struct osc_link *)ctx)->reason;
}
