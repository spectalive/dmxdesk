// desk_console.refresh for OSC: offers the surface again on the open socket;
// OSC cannot ask the console, so it carries the state heard since.
#ifndef OSC_CONSOLE_REFRESH_H
#define OSC_CONSOLE_REFRESH_H

#include <stdint.h>

void osc_console_refresh(void *ctx, int64_t now);

#endif
