// A number off the wire as an int in [lo, hi], rounded; NaN reads as lo.
#ifndef OSC_CLAMP_H
#define OSC_CLAMP_H

int osc_clamp(float v, int lo, int hi);

#endif
