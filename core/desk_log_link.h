// The link and its reason, logged when either changes.
#ifndef DESK_LOG_LINK_H
#define DESK_LOG_LINK_H

#include <stdint.h>

#include "desk_app.h"

void desk_log_link(struct desk_app *app, int64_t now);

#endif
