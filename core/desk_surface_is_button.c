#include "desk_surface_is_button.h"

int desk_surface_is_button(enum desk_surface_kind kind) {
    return kind == SURFACE_TOGGLE || kind == SURFACE_FLASH || kind == SURFACE_STOP_ALL ||
           kind == SURFACE_BUTTON;
}
