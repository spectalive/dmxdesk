// The page changed: taps and captures on the speed cards are forgotten and
// every hold is released.
#ifndef DESK_FOLLOW_PAGE_H
#define DESK_FOLLOW_PAGE_H

#include <stdint.h>

#include "desk_app.h"

void desk_follow_page(struct desk_app *app, int64_t now);

#endif
