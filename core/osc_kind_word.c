#include "osc_kind_word.h"

const char *osc_kind_word(enum desk_surface_kind kind) {
    switch (kind) {
    case SURFACE_GROUP:    return "group";
    case SURFACE_TOGGLE:   return "toggle";
    case SURFACE_FLASH:    return "flash";
    case SURFACE_STOP_ALL: return "stop-all";
    case SURFACE_BUTTON:   return "button";
    case SURFACE_LEVEL:    return "level";
    case SURFACE_DIAL:     return "dial";
    default:               return "other";
    }
}
