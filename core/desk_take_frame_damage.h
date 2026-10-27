// What the turn's frame must repaint: nothing while the display is blanked
// (the CRTC is off and a flip would only wake the pipeline the operator just
// switched off), the settings sheet when it changed, and the model's damage.
// Returns 1 when something is damaged; dw < 0 means all of it.
#ifndef DESK_TAKE_FRAME_DAMAGE_H
#define DESK_TAKE_FRAME_DAMAGE_H

#include "desk_app.h"

int desk_take_frame_damage(struct desk_app *app, int *dx, int *dy, int *dw, int *dh);

#endif
