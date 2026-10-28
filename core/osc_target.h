// What an OSC address names in a console's map: a control, a dial's
// multiplier, a function, or nothing the desk knows.
#ifndef OSC_TARGET_H
#define OSC_TARGET_H

enum osc_target_kind { OSC_TARGET_NONE, OSC_TARGET_ITEM, OSC_TARGET_FACTOR, OSC_TARGET_FUNCTION };

struct osc_target {
    enum osc_target_kind kind;
    int index;      // into the surface's items, or into the functions
};

#endif
