// A 32-bit word read in OSC's big-endian order.
#ifndef OSC_BE32_READ_H
#define OSC_BE32_READ_H

#include <stdint.h>

uint32_t osc_be32_read(const char *src);

#endif
