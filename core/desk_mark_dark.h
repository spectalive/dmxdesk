// Whether a mark drawn on a colour should be the ground's near-black rather
// than the ink's white: whichever has the higher WCAG contrast ratio.
#ifndef DESK_MARK_DARK_H
#define DESK_MARK_DARK_H

#include <stdint.h>

int desk_mark_dark(uint32_t colour);

#endif
