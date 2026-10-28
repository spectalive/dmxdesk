// A console reached over plain OSC, as its surface file describes it: OSC
// has no way to ask a console what it has, so the file declares the surface
// the desk validates against and the address each control answers to.
// address[i], factor_address[i] and ms_range[i] belong to surface.item[i];
// an empty address is a control the desk cannot drive.
#ifndef OSC_MAP_H
#define OSC_MAP_H

#include "desk_surface.h"
#include "osc_factor_press.h"
#include "osc_function.h"
#include "osc_msg.h"

struct osc_map {
    struct desk_surface surface;
    char (*address)[OSC_ADDRESS_MAX];
    char (*factor_address)[OSC_ADDRESS_MAX];   // a dial's multiplier, as a value
    int (*ms_range)[2];     // a dial whose time goes as 0..1 of [min, max]; -1 when not
    int *reports;           // 1 when the console reports this control's state back
    struct osc_factor_press *factor_press;     // multipliers set by a button each
    int factor_presses;
    struct osc_function *function;
    int functions;
    int grand_master;   // the item index of the grand master fader, or -1
    int feedback;       // 1 when the console reports its state back at all
};

#endif
