// Steps the QLC+ session and reports its link.
#ifndef QLC_CONSOLE_STEP_H
#define QLC_CONSOLE_STEP_H

#include <stdint.h>

#include "desk_console_link.h"

enum desk_console_link qlc_console_step(void *ctx, int64_t now);

#endif
