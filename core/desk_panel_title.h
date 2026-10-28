// The words a panel's heading starts with: its own, or the label of a mode
// switch laid inside it while that switch is on (COLOR DEL RIG reads GOLPES
// DE COLOR while hits are on).
#ifndef DESK_PANEL_TITLE_H
#define DESK_PANEL_TITLE_H

#include "desk_model.h"

const char *desk_panel_title(const struct desk_model *m, const struct desk_heading *h);

#endif
