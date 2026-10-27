#include "desk_colour_dim.h"

#include "desk_layout.h"

uint32_t desk_colour_dim(uint32_t colour) {
    return 0xFF000000u | (((colour >> 1) & 0x7F7F7Fu) + ((DESK_PANEL >> 1) & 0x7F7F7Fu));
}
