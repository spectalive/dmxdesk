#include "desk_speed_target_rect.h"

#include "desk_speed_layout.h"

struct desk_rect desk_speed_target_rect(int dial, enum speed_target t) {
    if (t == SPEED_T_BOTH)
        return (struct desk_rect){ SPEED_BOTH_X, SPEED_BOTH_Y, SPEED_BOTH_W, SPEED_BOTH_H };
    if (dial < 0 || dial > 1)
        return (struct desk_rect){ 0, 0, 0, 0 };
    int x = SPEED_CARD_X(dial) + SPEED_PAD, y = SPEED_CARD_Y, w = SPEED_CARD_W - 2 * SPEED_PAD;
    int half = (w - SPEED_CELL_GAP) / 2, right = x + w - half;
    switch (t) {
    case SPEED_T_TAP:        return (struct desk_rect){ x, y + SPEED_TAP_Y, w, SPEED_TAP_H };
    case SPEED_T_FACTOR_ONE: return (struct desk_rect){ x + w - SPEED_TIMES_W, y + SPEED_TIMES_Y,
                                                        SPEED_TIMES_W, SPEED_TIMES_H };
    case SPEED_T_BPM_DOWN:   return (struct desk_rect){ x, y + SPEED_ROW_Y(0), half, SPEED_ROW_H };
    case SPEED_T_BPM_UP:     return (struct desk_rect){ right, y + SPEED_ROW_Y(0), half, SPEED_ROW_H };
    case SPEED_T_HALF:       return (struct desk_rect){ x, y + SPEED_ROW_Y(1), half, SPEED_ROW_H };
    case SPEED_T_DOUBLE:     return (struct desk_rect){ right, y + SPEED_ROW_Y(1), half, SPEED_ROW_H };
    default:                 return (struct desk_rect){ 0, 0, 0, 0 };
    }
}
