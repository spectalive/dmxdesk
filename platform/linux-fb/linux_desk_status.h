// The battery and the Wi-Fi from the kernel; the backlight follows them,
// and an open settings surface shows the brightness, whether it was saved,
// and - without a supplicant - the configured network and the address.
#ifndef LINUX_DESK_STATUS_H
#define LINUX_DESK_STATUS_H

#include <stdint.h>

#include "desk_app.h"

void linux_desk_status(void *ctx, struct desk_app *app, struct status *next, int64_t now);

#endif
