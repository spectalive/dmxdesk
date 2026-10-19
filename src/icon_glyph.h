// The mark a map control's "icon" glyph draws. The map carries the glyph
// `qlctool deskmap` split off the console caption (a UTF-8 character such as
// "\xe2\x98\x81", a cloud); the desk draws its own vector mark for it rather
// than the character, since its fonts have none of them.
#ifndef ICON_GLYPH_H
#define ICON_GLYPH_H

#include "icon_data.h"

// The icon for `glyph`, or -1 when it is empty or one the desk does not
// know: an unknown glyph draws nothing. A trailing emoji presentation
// selector (U+FE0F) is ignored.
int icon_glyph(const char *glyph);

#endif
