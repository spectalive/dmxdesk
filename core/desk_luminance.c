#include "desk_luminance.h"

#include "desk_srgb_linear.h"

double desk_luminance(uint32_t colour) {
    return 0.2126 * desk_srgb_linear(colour >> 16 & 0xFF) +
           0.7152 * desk_srgb_linear(colour >> 8 & 0xFF) +
           0.0722 * desk_srgb_linear(colour & 0xFF);
}
