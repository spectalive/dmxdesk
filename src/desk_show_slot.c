#include "desk_show_slot.h"

int desk_show_slot(int x, int w, int n, int gap, int i, int *slot_w) {
    int room = w - (n - 1) * gap;
    int base = room / n, extra = room % n;
    *slot_w = base + (i < extra);
    return x + i * (base + gap) + (i < extra ? i : extra);
}
