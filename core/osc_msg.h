// One OSC message as the desk speaks it: an address and at most one number.
// Consoles take a button, a fader or a time as a single int32 or float32;
// a message whose first argument is anything else decodes with no number.
#ifndef OSC_MSG_H
#define OSC_MSG_H

#define OSC_ADDRESS_MAX 64

struct osc_msg {
    char address[OSC_ADDRESS_MAX];
    char type;      // 'i', 'f', or 0 for no number
    int i;
    float f;
};

#endif
