// The chrome a finger can reach outside the content: the tabs, the gear,
// the lock, the pager. Rectangles shared by painting and hit testing, so a
// thing is pressed exactly where it is drawn.
#ifndef DESK_VIEW_H
#define DESK_VIEW_H

#include "desk_layout_resolve.h"

struct desk_rect { int x, y, w, h; };

// The target of tab `index` (enum desk_tab): 136x48 with 8 px gaps from x 16,
// the whole height of the bar; its pill is drawn inside, 4 px from the bar's edges.
struct desk_rect desk_view_tab(int index);
// The gear and the lock: 48x48 targets in the bar.
struct desk_rect desk_view_gear(void);
struct desk_rect desk_view_lock_target(void);
// Label-sized segments fill the reserved strip under the page's content, with
// the same hit and paint boxes.
struct desk_rect desk_view_pager(int index, const struct desk_layout *layout, int page);

int desk_rect_contains(struct desk_rect r, int x, int y);

#endif
