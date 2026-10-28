// The bytes an OSC string of n characters takes on the wire: its NUL and
// enough more to end on a four-byte boundary.
#ifndef OSC_PADDED_H
#define OSC_PADDED_H

#include <stddef.h>

size_t osc_padded(size_t n);

#endif
