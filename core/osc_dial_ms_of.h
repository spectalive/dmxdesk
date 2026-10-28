// A dial's time from the console's message: a float as 0..1 of range when
// the dial has one, otherwise the number as ms, never below 0.
#ifndef OSC_DIAL_MS_OF_H
#define OSC_DIAL_MS_OF_H

#include "osc_msg.h"

int osc_dial_ms_of(const int range[2], const struct osc_msg *msg);

#endif
