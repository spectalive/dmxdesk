// What QLC+ must listen to for an OSC surface file to drive it: one line
// per external input, "<widget id>\t<input id>\t<channel>\t<path>", with
// the channel QLC+'s OSC plugin derives from the path. A control's own
// address is input 0 (a button's press, a fader's level, a dial's wheel);
// a dial's multiplier buttons are its inputs 4 and 6-13. Lines written, or
// -1 when writing fails.
#ifndef QLC_OSC_CHANNELS_WRITE_H
#define QLC_OSC_CHANNELS_WRITE_H

#include <stdio.h>

#include "osc_map.h"

int qlc_osc_channels_write(const struct osc_map *map, FILE *out);

#endif
