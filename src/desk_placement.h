// A control's place on one bank of one page. Geometry lives here and nowhere
// else: a control is a fact about the show, a placement a fact about the
// screen, and the same control (AUTO, say) has a placement on every page.
#ifndef DESK_PLACEMENT_H
#define DESK_PLACEMENT_H

enum desk_tile {
    TILE_CUE,       // 64 tall, a hook, a toggle, a chase: four to a family's row
    TILE_WIDE,      // the cue's size; kept for the hand-laid tests
    TILE_SWATCH,    // a pick: its colour as a square over its name, or its mark
    TILE_NUMBER,    // a numbered pick: the number large over its caption
    TILE_TRAJECTORY,// a heads' pick: the figure its beams draw over its name
    TILE_COMPACT,   // 128x64, a small button
    TILE_HAZE,      // 200x64, an ambient rhythm
    TILE_HOLD,      // 128x104, a hit: held on the Mac, or fired while held here
    TILE_STATE,     // 84x104, one of the room's states in SALA AHORA
    TILE_FOG,       // a fog hold in GOLPES, the hold look in teal
    TILE_SEGMENT,   // one equal option of HUMO AMBIENTE, 48 tall
    TILE_MINI,      // a rig colour in COLOR DEL RIG: the colour, no words
    TILE_TEMPO,     // the TEMPO SHOW panel: the show dial's BPM and a tap target
    TILE_MASTER,    // the fader in the right column
    TILE_PANIC,     // the stop-all under it
};

struct desk_placement {
    int control;    // index into the model's controls
    int page, bank;
    int x, y, w, h;
    enum desk_tile tile;
};

#endif
