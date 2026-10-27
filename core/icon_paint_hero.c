#include "icon_paint_hero.h"

#include "canvas_blend.h"

int icon_paint_hero(struct canvas *c, enum icon_id id, int x, int y, uint32_t colour) {
    if (id < 0 || id >= ICON_COUNT || ICON_HERO_OF[id] < 0)
        return -1;
    const uint8_t *a = ICON_HERO_ALPHA[ICON_HERO_OF[id]];
    for (int j = 0; j < ICON_HERO_SIZE; j++)
        for (int i = 0; i < ICON_HERO_SIZE; i++) {
            uint8_t alpha = a[j * ICON_HERO_SIZE + i];
            if (alpha)
                canvas_blend(c, x + i, y + j, ((uint32_t)alpha << 24) | (colour & 0xFFFFFF));
        }
    return 0;
}
