// Equal slots across a row: `n` tiles with `gap` between them fill `w` from
// `x`, the pixels left over by the division spread one each over the first
// slots so the row ends flush. Returns slot `i`'s x and sets its width.
#ifndef DESK_SHOW_SLOT_H
#define DESK_SHOW_SLOT_H

int desk_show_slot(int x, int w, int n, int gap, int i, int *slot_w);

#endif
