#include "icon_paint_mid.h"

#include "canvas_blend.h"

void icon_paint_mid(struct canvas *c, enum icon_id id, int x, int y, uint32_t colour) {
    if (id < 0 || id >= ICON_COUNT)
        return;
    const uint8_t *a = ICON_MID_ALPHA[id];
    for (int j = 0; j < ICON_MID_SIZE; j++)
        for (int i = 0; i < ICON_MID_SIZE; i++) {
            uint8_t alpha = a[j * ICON_MID_SIZE + i];
            if (alpha)
                canvas_blend(c, x + i, y + j, ((uint32_t)alpha << 24) | (colour & 0xFFFFFF));
        }
}
