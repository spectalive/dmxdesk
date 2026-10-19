#include "desk_paint_trajectory.h"

#include "desk_caption.h"
#include "desk_paint_text_centred.h"
#include "desk_paint_tile_base.h"
#include "desk_trajectory_of.h"
#include "icon_paint_large.h"

void desk_paint_trajectory(struct canvas *c, const struct desk_control *ctl,
                           const struct desk_placement *p, const struct desk_fonts *fonts,
                           enum desk_link link) {
    uint32_t ink = desk_paint_tile_base(c, ctl, p, link);
    struct desk_caption_lines lines;
    desk_caption_fit(fonts->tile_s, p->w - 8, ctl->label, &lines);
    int pitch = (fonts->tile_s ? font_height(fonts->tile_s) : 15) + 2;
    int top = p->y + (p->h - ICON_LARGE_SIZE - 2 - lines.lines * pitch) / 2;
    icon_paint_large(c, (enum icon_large_id)desk_trajectory_of(ctl->label),
                     p->x + (p->w - ICON_LARGE_SIZE) / 2, top, ink);
    for (int i = 0; i < lines.lines; i++)
        desk_paint_text_centred(c, fonts->tile_s, p->x + 4, top + ICON_LARGE_SIZE + 2 + i * pitch,
                                p->w - 8, pitch, lines.line[i], ink);
}
