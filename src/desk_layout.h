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
#define DESK_MASTER_TILE_X DESK_MASTER_X
#define DESK_MASTER_TILE_Y 56
#define DESK_MASTER_TILE_W DESK_MASTER_W
#define DESK_MASTER_TILE_H 372
#define DESK_PANIC_Y 440
#define DESK_PANIC_H 144

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
// DIRECTO: four flat panels down the content, 8 px apart. SALA AHORA
// (128) carries the running state's name in a block at its left and the
// seven states beside it; GOLPES (152) the holds in one row; HUMO AMBIENTE
// (96) the segmented selector; the bottom row (128) COLOR DEL RIG with the
// rig's ten colours and, at its right, the TEMPO SHOW panel.
#define DESK_ROOM_Y 56
#define DESK_ROOM_H 128
#define DESK_ROOM_BLOCK_W 156
#define DESK_STATE_W 84
#define DESK_STATE_H 104
#define DESK_HITS_Y 192
#define DESK_HITS_H 152
#define DESK_HAZE_Y 352
#define DESK_HAZE_H 96
#define DESK_SEGMENT_H 48
#define DESK_RIG_Y 456
#define DESK_RIG_H 128
#define DESK_MINI_H 64
#define DESK_TEMPO_W 232
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

// The palette, flat. Accent blue means the master says running (or, for
// OFF, that nothing runs) and nothing else; the stop is the one red; a
// hold-to-fire button has its own violet field. Tiles sit on panels, panels
// on the ground, each a step lighter, a tile drawn with a 1 px keyline.
#define DESK_GLASS      0xFF0C0E12u   // the ground
#define DESK_PANEL      0xFF14181Eu   // a section's panel
#define DESK_TILE       0xFF1B1F27u
#define DESK_RAISED     0xFF262C36u
#define DESK_LINE       0xFF39404Du   // keylines and rules
#define DESK_INK        0xFFF5F7FAu
#define DESK_MUTED      0xFFADB6C4u
#define DESK_ACCENT     0xFF1A7CFFu   // running, and the tab or family on screen
#define DESK_ACCENT_INK 0xFFFFFFFFu
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
