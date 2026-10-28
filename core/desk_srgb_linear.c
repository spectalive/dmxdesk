#include "desk_srgb_linear.h"

#include <math.h>

double desk_srgb_linear(unsigned channel) {
    double c = channel / 255.0;
    return c <= 0.04045 ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
}
