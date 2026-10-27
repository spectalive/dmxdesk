// The supplicant's turn: a scan's end fills the card, every event line
// reaches a running join (which also watches the address), and a queued
// scan's busy word and clock start when the command actually goes out.
#ifndef LINUX_DESK_SUPPLICANT_H
#define LINUX_DESK_SUPPLICANT_H

#include <stdint.h>

#include "desk_app.h"
#include "linux_desk.h"

void linux_desk_supplicant(struct linux_desk *d, struct desk_app *app, int64_t now);

#endif
