#include "osc_be32_write.h"

void osc_be32_write(char *dst, uint32_t v) {
    dst[0] = (char)(v >> 24);
    dst[1] = (char)(v >> 16);
    dst[2] = (char)(v >> 8);
    dst[3] = (char)v;
}
