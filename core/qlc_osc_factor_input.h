// The external input of a QLC+ speed dial that sets a multiplier: the
// desk's factor (QLC+'s SpeedMultiplier enum, 2 = 1/16x .. 10 = 16x) as the
// dial's input id, 6-9 for 1/16x-1/2x, 4 (Reset) for 1x, 10-13 for 2x-16x.
// -1 for a factor no input sets.
#ifndef QLC_OSC_FACTOR_INPUT_H
#define QLC_OSC_FACTOR_INPUT_H

int qlc_osc_factor_input(int factor);

#endif
