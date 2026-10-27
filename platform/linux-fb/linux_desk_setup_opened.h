// The settings surface opened: a scan starts unless one, or a join, is
// already under way.
#ifndef LINUX_DESK_SETUP_OPENED_H
#define LINUX_DESK_SETUP_OPENED_H

#include <stdint.h>

#include "desk_app.h"

void linux_desk_setup_opened(void *ctx, struct desk_app *app, int64_t now);

#endif
