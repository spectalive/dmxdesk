// What a lighting console offers the desk, in the desk's own terms: one item
// per control the console has, keyed by the console's own id for it. A
// console adapter builds it from whatever the console serves (QLC+'s
// /vc.json for qlc_console); the desk validates its show map against it and
// reads each control's state from it. Nothing here names a console.
#ifndef DESK_SURFACE_H
#define DESK_SURFACE_H

#define DESK_SURFACE_VERSION_MAX 16

enum desk_surface_kind {
    SURFACE_OTHER,      // something the desk does not drive: a label, a pad
    SURFACE_GROUP,      // a container of other controls: a frame, a page of one
    SURFACE_TOGGLE,     // a button that latches its function on and off
    SURFACE_FLASH,      // a button that holds its function while pressed
    SURFACE_STOP_ALL,   // the console's panic: stops every function
    SURFACE_BUTTON,     // a button with any other action, such as a blackout
    SURFACE_LEVEL,      // a fader carrying a 0-255 level
    SURFACE_DIAL,       // a speed dial: a time and a multiplier
};

struct desk_surface_item {
    int id;             // the console's address for the control
    enum desk_surface_kind kind;
    int group_id;       // the console's id for what holds it, or -1
    int function_id;    // the function it drives, or -1; 0 is a real function
    int disabled;       // the console refuses it
    int on;             // a button: pressed or latched
    int level;          // a fader: its level
    int flash_max_ms;   // a flash: the console's own hold limit, 0 when none
    int flash_priority; // a flash: it overrides whatever else drives its channels
    int dial_ms;        // a dial: the time it shows, -1 when unknown
    int dial_factor;    // a dial: the multiplier it applies
    int dial_max_ms;    // a dial: the longest time it allows, -1 when unknown
};

struct desk_surface {
    char version[DESK_SURFACE_VERSION_MAX];  // the console software's, "" when unknown
    int count;
    struct desk_surface_item *item;          // owned, `count` long, ascending by id
};

#endif
