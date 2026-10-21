// Fixed chrome of the desk at 1024x600, third design: a bar of three tabs
// (DIRECTO, EFECTOS, RITMO) and the status across the top, the content under
// it, the master column at the right; EFECTOS adds a column of its families
// at the left. Shared by painting and hit testing so a tile is drawn exactly where
// it is pressed. Everything sits on an 8 px spacing system; the exceptions
// (segment gap 4) are named where they are used.
#ifndef DESK_LAYOUT_H
#define DESK_LAYOUT_H

#define DESK_W 1024
#define DESK_H 600

// The bar: three tabs from the left, the status cluster and the two targets
// at the right. A tab is a 136x48 target whose pill is drawn 40 tall, 4 px in
// from the bar's edges. Gear and lock are 48x48 targets around 24 px icons.
#define DESK_BAR_H 48
#define DESK_TAB_X 16
#define DESK_TAB_Y 0
#define DESK_TAB_W 136
#define DESK_TAB_H 48
#define DESK_TAB_GAP 8
#define DESK_TAB_INSET 4
// The status word ends here, the link dot right after it.
#define DESK_STATUS_END 748
#define DESK_LINK_DOT_X 756
#define DESK_LINK_DOT 8
#define DESK_GEAR_X 912
#define DESK_LOCK_X 960
#define DESK_TARGET 48

// The content and the master column.
#define DESK_CONTENT_X 16
#define DESK_CONTENT_W 828
#define DESK_CONTENT_Y 56
#define DESK_CONTENT_END 584
#define DESK_MASTER_X 860
#define DESK_MASTER_W 148
#define DESK_MASTER_TILE_X (DESK_MASTER_X + 12)
#define DESK_MASTER_TILE_Y 56
#define DESK_MASTER_TILE_W 124
#define DESK_MASTER_TILE_H 252
#define DESK_PANIC_Y 320
#define DESK_PANIC_H 88

// EFECTOS: one target per family down the left, at most 64 tall, and the
// family's page in what is left of the content.
#define DESK_FAMILY_X 16
#define DESK_FAMILY_W 124
#define DESK_FAMILY_H 64
#define DESK_EFFECTS_X 148
#define DESK_EFFECTS_W (DESK_CONTENT_X + DESK_CONTENT_W - DESK_EFFECTS_X)

// A flowed section sits on a flat panel, its heading inside at the top.
#define DESK_PANEL_PAD 12
#define DESK_PANEL_RADIUS 16

#define DESK_GAP 8
#define DESK_RADIUS 12

// Tile sizes: a hook or state 200x64, four to a row; a pick 128x88, six to
// a row; a hold 128x104. Older names kept for the tests that lay by hand.
#define DESK_HOOK_W 200
#define DESK_HOOK_H 64
#define DESK_PICK_W 128
#define DESK_PICK_H 88
#define DESK_HOLD_TILE_H 104
#define DESK_COLS 6
// DIRECTO puts the seven mode buttons and tempo above the compact room
// readout and effects. The rig's 17 colours fill the lower left in three rows;
// ambient haze sits below effects on the right.
#define DESK_HERO_X DESK_CONTENT_X
#define DESK_HERO_Y 160
#define DESK_HERO_W 516
#define DESK_HERO_H 114
#define DESK_STATE_Y DESK_CONTENT_Y
#define DESK_STATE_H 96
#define DESK_STATE_GAP 6
#define DESK_HITS_X 540
#define DESK_HITS_W (DESK_CONTENT_X + DESK_CONTENT_W - DESK_HITS_X)
#define DESK_HITS_Y 160
#define DESK_HITS_TOP 184
#define DESK_HITS_END 424
#define DESK_HOLD_WIDE_H 56
#define DESK_RIG_Y 282
#define DESK_RIG_H (DESK_CONTENT_END - DESK_RIG_Y)
#define DESK_HAZE_Y 432
#define DESK_HAZE_H (DESK_CONTENT_END - DESK_HAZE_Y)
#define DESK_HAZE_W DESK_HITS_W
#define DESK_HAZE_OFF_W 60
#define DESK_TEMPO_X DESK_HITS_X
#define DESK_TEMPO_Y DESK_CONTENT_Y
#define DESK_TEMPO_W DESK_HITS_W
#define DESK_TEMPO_H 96
// Minimum width for a rig colour tile.
#define DESK_MINI_MIN_W 44
// A panel's heading: 16 px tall, 8 px over the tiles under it.
#define DESK_PANEL_TOP (DESK_PANEL_PAD + 16 + DESK_GAP)
#define DESK_GRID_X DESK_CONTENT_X
#define DESK_GRID_Y DESK_CONTENT_Y
#define DESK_TILE_W DESK_HOOK_W
#define DESK_TILE_H DESK_HOOK_H

// The pager fills the existing 56 px strip; six minimum widths plus five
// 8 px gaps fit an EFECTOS page without taking any room from the content.
#define DESK_PAGER_MIN_W 104
#define DESK_PAGER_H 56
#define DESK_PAGER_Y (DESK_CONTENT_END - DESK_PAGER_H)

// The palette, flat. What runs (or, for OFF, that nothing runs) and the tab
// or family on screen wear the selected look: a frosted lighter fill inside
// a brighter hairline, a lighter top edge, white words and a short accent
// bar; a colour pick is ringed in the selected line instead, so its colour
// stays whole. The accent is a calm indigo, the one hue besides the show's:
// the selection bar, the hero's edge, a bank's running dot. The hero fills
// with a deep indigo while a state runs (white on it 10:1). The stop is the
// one red; a hold-to-fire button has its own violet field. Tiles sit on
// panels, panels on the ground, each a step lighter, with a 1 px keyline.
#define DESK_GLASS      0xFF0C0E12u   // the ground
#define DESK_PANEL      0xFF14181Eu   // a section's panel
#define DESK_TILE       0xFF1B1F27u
#define DESK_RAISED     0xFF262C36u
#define DESK_LINE       0xFF39404Du   // keylines and rules
#define DESK_INK        0xFFF5F7FAu
#define DESK_MUTED      0xFFADB6C4u
#define DESK_SELECTED_FILL 0xFF343A46u   // white ~12% over a tile
#define DESK_SELECTED_LINE 0xFF636A78u   // white ~28%: the hairline, a pick's ring
#define DESK_SELECTED_TOP  0xFF7A808Cu   // the glass's lit top edge
#define DESK_ACCENT     0xFF7C86D9u   // calm indigo: the selection bar and small marks
#define DESK_ACCENT_INK 0xFFFFFFFFu   // words on the selected look and on the hero
#define DESK_HERO_FILL  0xFF2E3A7Eu   // SALA AHORA while a state runs
#define DESK_HERO_SOFT  0xFFD6DAF8u   // the hero's quieter line (7:1 on its fill)
#define DESK_OK         0xFF3EE0AAu   // the link dot when the master answers
#define DESK_WARN       0xFFD42A2Au   // PARAR TODO and warnings
#define DESK_WARN_INK   0xFFFFD2CCu   // the quieter line on the red
#define DESK_HOLD_FILL  0xFF1F1B2Bu
#define DESK_HOLD_LINE  0xFF7860C8u
// Fog holds keep a teal field of their own, so smoke never reads as light.
#define DESK_FOG_FILL   0xFF14231Fu
#define DESK_FOG_LINE   0xFF3FA88Au
// The ground at 69% over what a modal dialog covers.
#define DESK_SCRIM      (0xB0000000u | (DESK_GLASS & 0xFFFFFFu))

#endif
