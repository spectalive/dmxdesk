#include "desk_speed_paint.h"

void desk_speed_tempo_step_rect(int x, int y, int w, int h, enum speed_target step,
                                int *tx, int *ty, int *tw, int *th) {
    *tw = 44;
    *th = 48;
    *tx = x + w - 12 - 88 - 8 - (step == SPEED_T_BPM_DOWN ? 96 : 48);
    *ty = y + (h - *th) / 2;
}
