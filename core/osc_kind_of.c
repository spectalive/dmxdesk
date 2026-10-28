#include "osc_kind_of.h"

#include <string.h>

int osc_kind_of(const char *word, enum desk_surface_kind *out) {
    static const struct { const char *word; enum desk_surface_kind kind; } kinds[] = {
        { "other", SURFACE_OTHER }, { "group", SURFACE_GROUP }, { "toggle", SURFACE_TOGGLE },
        { "flash", SURFACE_FLASH }, { "stop-all", SURFACE_STOP_ALL },
        { "button", SURFACE_BUTTON }, { "level", SURFACE_LEVEL }, { "dial", SURFACE_DIAL },
    };
    for (size_t i = 0; i < sizeof kinds / sizeof kinds[0]; i++)
        if (!strcmp(word, kinds[i].word)) {
            *out = kinds[i].kind;
            return 0;
        }
    return -1;
}
