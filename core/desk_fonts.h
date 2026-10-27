// The desk's type scale, one face each, opened from the bundled Inter.
#ifndef DESK_FONTS_H
#define DESK_FONTS_H

#include "font.h"

// The type scale, one face each. `desk_fonts_open` fills it from a directory
// holding Inter-SemiBold.ttf and Inter-Regular.ttf; a face that fails to open
// is NULL and the painter falls back to the 3x5 glyphs.
struct desk_fonts {
    struct font *tile;      // SemiBold 17: a control's name
    struct font *tile_s;    // SemiBold 14: a small control's name
    struct font *value;     // SemiBold 30: readouts
    struct font *big;       // SemiBold 44: the tempo, the room's state
    struct font *huge;      // SemiBold 72: RITMO's BPM
    struct font *tab;       // SemiBold 15: the tabs
    struct font *section;   // SemiBold 12: section titles, capitals
    struct font *label;     // Regular 15: settings rows, banners
    struct font *small;     // Regular 13: subtitles, notes
    struct font *tiny;      // Regular 11: the detail on a state tile
};

int desk_fonts_open(struct desk_fonts *f, const char *dir);
void desk_fonts_close(struct desk_fonts *f);

#endif
