// An OSC message off the wire: its address and its first argument when that
// is an int32 or a float32. 0, or -1 for anything malformed, truncated, or a
// bundle. A message with no type tag, the OSC 1.0 leniency, has no number.
#ifndef OSC_DECODE_H
#define OSC_DECODE_H

#include <stddef.h>

#include "osc_msg.h"

int osc_decode(const char *buf, size_t len, struct osc_msg *out);

#endif
