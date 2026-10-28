#include "osc_dial_fraction.h"

float osc_dial_fraction(const int range[2], int ms) {
    if (ms <= range[0])
        return 0.0f;
    if (ms >= range[1])
        return 1.0f;
    return (float)(ms - range[0]) / (float)(range[1] - range[0]);
}
