// A 32-bit word in OSC's big-endian order.
#ifndef OSC_BE32_WRITE_H
#define OSC_BE32_WRITE_H

#include <stdint.h>

void osc_be32_write(char *dst, uint32_t v);

#endif
