#ifndef DESK_PAINT_ROOM_STATUS_H
#define DESK_PAINT_ROOM_STATUS_H

#include "canvas.h"
#include "desk_fonts.h"
#include "desk_model.h"

void desk_paint_room_status(struct canvas *canvas, const struct desk_model *model,
                            int x, int y, int width, const struct desk_fonts *fonts);

#endif
