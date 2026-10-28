#include "desk_mark_dark.h"

#include "desk_layout.h"
#include "desk_luminance.h"

// A luma threshold put pure green, 0x00ff00, exactly on the line, and it kept
// a white bolt nobody could see (owner, 2026-10-28). Contrast ratios decide.
int desk_mark_dark(uint32_t colour) {
    double l = desk_luminance(colour);
    double dark = (l + 0.05) / (desk_luminance(DESK_GLASS) + 0.05);
    double white = (desk_luminance(DESK_INK) + 0.05) / (l + 0.05);
    return dark > white;
}
