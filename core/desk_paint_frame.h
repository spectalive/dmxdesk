// Paints the desk into its canvas: the damaged rectangle only when dw > 0,
// since every primitive clips to it. For a backend that draws pixels itself.
#ifndef DESK_PAINT_FRAME_H
#define DESK_PAINT_FRAME_H

#include "desk_app.h"

void desk_paint_frame(struct desk_app *app, int dx, int dy, int dw, int dh);

#endif
