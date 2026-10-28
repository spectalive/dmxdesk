// The WCAG 2 relative luminance of a 0xRRGGBB colour, 0 to 1.
#ifndef DESK_LUMINANCE_H
#define DESK_LUMINANCE_H

#include <stdint.h>

double desk_luminance(uint32_t colour);

#endif
