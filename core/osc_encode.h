// An OSC message on the wire: the padded address, the padded type tag, and
// the number if there is one. Its length, or -1 when the address is not an
// OSC address, the type is not 'i', 'f' or 0, or cap is too small.
#ifndef OSC_ENCODE_H
#define OSC_ENCODE_H

#include <stddef.h>

#include "osc_msg.h"

int osc_encode(const struct osc_msg *msg, char *buf, size_t cap);

#endif
