#include "icon_glyph.h"

#include <string.h>

#define PRESENTATION_SELECTOR "\xef\xb8\x8f"

int icon_glyph(const char *glyph) {
    if (!glyph || !glyph[0])
        return -1;
    size_t len = strlen(glyph);
    size_t sel = sizeof PRESENTATION_SELECTOR - 1;
    if (len > sel && strcmp(glyph + len - sel, PRESENTATION_SELECTOR) == 0)
        len -= sel;
    for (size_t i = 0; i < sizeof ICON_GLYPHS / sizeof ICON_GLYPHS[0]; i++)
        if (strlen(ICON_GLYPHS[i].glyph) == len && memcmp(ICON_GLYPHS[i].glyph, glyph, len) == 0)
            return ICON_GLYPHS[i].id;
    return -1;
}
