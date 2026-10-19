#include "desk_paint_number.h"

#include "desk_caption.h"
#include "desk_caption_number.h"
#include "desk_layout.h"
#include "desk_paint_text_centred.h"
#include "desk_paint_tile_base.h"

void desk_paint_number(struct canvas *c, const struct desk_control *ctl,
                       const struct desk_placement *p, const struct desk_fonts *fonts,
                       enum desk_link link) {
    uint32_t ink = desk_paint_tile_base(c, ctl, p, link);
    const char *number = desk_caption_number(ctl->label);
    if (!number)
        number = ctl->label;
    // The readout face while the number fits across, the tile face when not.
    struct font *face = fonts->value;
    if (face && font_width(face, number) > p->w - 16)
        face = fonts->tile;
    int number_h = face ? font_height(face) : 15;
    if (number == ctl->label) {
        desk_paint_text_centred(c, face, p->x + 8, p->y, p->w - 16, p->h, number, ink);
        return;
    }
    struct desk_caption_lines lines;
    desk_caption_fit(fonts->small, p->w - 8, ctl->label, &lines);
    int pitch = (fonts->small ? font_height(fonts->small) : 15) + 1;
    int top = p->y + (p->h - number_h - 2 - lines.lines * pitch) / 2;
    desk_paint_text_centred(c, face, p->x + 8, top, p->w - 16, number_h, number, ink);
    uint32_t quiet = ink == DESK_INK ? DESK_MUTED : ink;
    for (int i = 0; i < lines.lines; i++)
        desk_paint_text_centred(c, fonts->small, p->x + 4, top + number_h + 2 + i * pitch, p->w - 8,
                                pitch, lines.line[i], quiet);
}
