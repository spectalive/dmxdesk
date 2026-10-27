// A Wi-Fi scan queued behind whatever the supplicant is doing; the card
// says "Scanning" from now.
#ifndef LINUX_DESK_QUEUE_SCAN_H
#define LINUX_DESK_QUEUE_SCAN_H

#include <stdint.h>

#include "desk_app.h"

void linux_desk_queue_scan(void *ctx, struct desk_app *app, int64_t now);

#endif
