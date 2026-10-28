#include "osc_clamp.h"

#include <math.h>

int osc_clamp(float v, int lo, int hi) {
    if (!(v >= (float)lo))
        return lo;
    if (v >= (float)hi)
        return hi;
    return (int)lroundf(v);
}
