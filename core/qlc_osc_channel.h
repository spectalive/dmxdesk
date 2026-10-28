// The input channel QLC+'s OSC plugin gives an OSC path: the CRC-16 its
// Utils::getChecksum takes of the path's bytes (plugins/interfaces/utils.h).
// A Virtual Console widget listens to a path by having this channel, on the
// OSC input's universe, as an external input.
#ifndef QLC_OSC_CHANNEL_H
#define QLC_OSC_CHANNEL_H

#include <stdint.h>

uint16_t qlc_osc_channel(const char *path);

#endif
