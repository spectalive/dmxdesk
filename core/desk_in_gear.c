#include "desk_in_gear.h"

#include "desk_view.h"

int desk_in_gear(int x, int y) {
    return desk_rect_contains(desk_view_gear(), x, y);
}
