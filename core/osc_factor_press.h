// A dial multiplier a console sets by a button of its own rather than by a
// value: pressing address sets the dial at item to factor.
#ifndef OSC_FACTOR_PRESS_H
#define OSC_FACTOR_PRESS_H

#include "osc_msg.h"

struct osc_factor_press {
    int item;       // the dial's index in the surface
    int factor;     // the desk's multiplier, 0..10
    char address[OSC_ADDRESS_MAX];
};

#endif
