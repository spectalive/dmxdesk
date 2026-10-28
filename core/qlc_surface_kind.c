#include "qlc_surface_kind.h"

enum desk_surface_kind qlc_surface_kind(const struct vc_widget *w) {
    switch (w->type_id) {
    case VC_BUTTON:
        return w->action_type == VC_TOGGLE ? SURFACE_TOGGLE
             : w->action_type == VC_FLASH ? SURFACE_FLASH
             : w->action_type == VC_STOP_ALL ? SURFACE_STOP_ALL : SURFACE_BUTTON;
    case VC_SLIDER:
        return SURFACE_LEVEL;
    case VC_SPEED_DIAL:
        return SURFACE_DIAL;
    case VC_FRAME:
    case VC_SOLO_FRAME:
        return SURFACE_GROUP;
    default:
        return SURFACE_OTHER;
    }
}
