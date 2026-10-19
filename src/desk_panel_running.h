// The control the master says runs among those laid inside panel `h`, or
// NULL when none does (or nothing is known).
#ifndef DESK_PANEL_RUNNING_H
#define DESK_PANEL_RUNNING_H

#include "desk_model.h"

const struct desk_control *desk_panel_running(const struct desk_model *m, const struct desk_heading *h);

#endif
