#include "osc_be32_read.h"

uint32_t osc_be32_read(const char *src) {
    const unsigned char *b = (const unsigned char *)src;
    return (uint32_t)b[0] << 24 | (uint32_t)b[1] << 16 | (uint32_t)b[2] << 8 | b[3];
}
