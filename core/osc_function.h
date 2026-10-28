// A function the console starts and stops over OSC by an address of its
// own, apart from any button: what the desk's bursts drive.
#ifndef OSC_FUNCTION_H
#define OSC_FUNCTION_H

#include "osc_msg.h"

struct osc_function {
    int id;
    char address[OSC_ADDRESS_MAX];
    int running;
};

#endif
