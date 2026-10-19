// Appends one of SHOW's panels: the flat field at (x, y, w, h) with its
// heading inside at the top. `section` is the LIVE page's section the panel
// shows, or -1 when its tiles come from elsewhere. Returns 0, or -1 when
// the layout holds no more headings.
#ifndef DESK_SHOW_PANEL_H
#define DESK_SHOW_PANEL_H

#include "desk_layout_resolve.h"

int desk_show_panel(struct desk_layout *out, int page, int x, int y, int w, int h,
                    const char *text, int section, enum desk_panel kind);

#endif
