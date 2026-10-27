// The services' turn: the supplicant when there is one, then the sweep.
#ifndef LINUX_DESK_SERVICE_H
#define LINUX_DESK_SERVICE_H

#include <stdint.h>

#include "desk_app.h"

void linux_desk_service(void *ctx, struct desk_app *app, int64_t now);

#endif
