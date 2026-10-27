// The desk's clock: CLOCK_MONOTONIC in milliseconds.
#ifndef LINUX_DESK_NOW_H
#define LINUX_DESK_NOW_H

#include <stdint.h>

int64_t linux_desk_now(void *ctx);

#endif
