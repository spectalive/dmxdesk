// SOURCES: desk_mark_dark.c desk_luminance.c desk_srgb_linear.c showmap.c showmap_release_to_parse.c showmap_release_to_prune.c
// The bolt on a colour-hit swatch takes the ink with the higher contrast.
// 2026-10-28: a luma threshold of 150 left pure green, luma 149.7, with a
// white bolt at 1.28:1 that the owner could not see on the Mac desk.

#include <assert.h>

#include "desk_layout.h"
#include "desk_luminance.h"
#include "desk_mark_dark.h"
#include "showmap.h"

static double contrast(uint32_t a, uint32_t b) {
    double la = desk_luminance(a), lb = desk_luminance(b);
    return la > lb ? (la + 0.05) / (lb + 0.05) : (lb + 0.05) / (la + 0.05);
}

int main(void) {
    assert(desk_mark_dark(0x00ff00u));
    assert(desk_mark_dark(0xffffffu) && !desk_mark_dark(0x000000u));
    assert(!desk_mark_dark(0x0000ffu));
    // Every colour the Vibra desk paints gets a mark at the WCAG 4.5:1 text
    // minimum, so the bolt is legible, not merely present.
    static struct show_map map;
    assert(showmap_load("show/vibra.desk.json", &map) == 0);
    int checked = 0;
    for (int i = 0; i < map.count; i++)
        for (int j = 0; j < map.control[i].swatches; j++) {
            uint32_t colour = map.control[i].swatch[j] & 0xffffffu;
            uint32_t mark = desk_mark_dark(colour) ? DESK_GLASS : DESK_INK;
            assert(contrast(colour, mark & 0xffffffu) >= 4.5);
            checked++;
        }
    assert(checked >= 18);
    return 0;
}
