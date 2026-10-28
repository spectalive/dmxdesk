// Re-reads /vc.json on the open connection.
#ifndef QLC_CONSOLE_REFRESH_H
#define QLC_CONSOLE_REFRESH_H

#include <stdint.h>

void qlc_console_refresh(void *ctx, int64_t now);

#endif
