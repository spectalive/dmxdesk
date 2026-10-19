// RITMO's rectangles: two equal cards side by side across the content, one
// per dial, and the Tap both strip under them. Inside a card, under the
// caption and the readout: a wide Tap, then -1 BPM / +1 BPM, then
// Más lento x2 / Más rápido x2; the multiplier sits at the readout's right
// as its own target, which returns the dial to x1.
#ifndef DESK_SPEED_LAYOUT_H
#define DESK_SPEED_LAYOUT_H

#define SPEED_CARD_W 410
#define SPEED_CARD_H 456
#define SPEED_CARD_X(i) (16 + (i) * (SPEED_CARD_W + 8))     // 16, 434
#define SPEED_CARD_Y 56
#define SPEED_RADIUS 16
#define SPEED_PAD 16

#define SPEED_TIMES_W 120
#define SPEED_TIMES_Y 52
#define SPEED_TIMES_H 48
#define SPEED_TAP_Y 168
#define SPEED_TAP_H 96
#define SPEED_ROW_Y(row) (276 + (row) * 84)                // 276, 360
#define SPEED_ROW_H 72
#define SPEED_CELL_GAP 8

#define SPEED_BOTH_X 16
#define SPEED_BOTH_Y 520
#define SPEED_BOTH_W 828
#define SPEED_BOTH_H 64

#endif
