// The trajectory a heads' pick draws, read from its caption. qlctool's EFX
// generator names a figure by its shape word first (its locales' shape_*:
// Circulo, Ocho, Linea, Diamante, Cuadrado, Hoja, Lissajous) and, for a
// group, the mode word second (mode_together "Simultaneo", mode_alternating
// "Alternado"). Any caption that does not lead with a shape word has no
// trajectory and keeps its glyph.
#ifndef DESK_TRAJECTORY_OF_H
#define DESK_TRAJECTORY_OF_H

// Returns the enum icon_large_id of the caption's mark, or -1.
int desk_trajectory_of(const char *caption);

#endif
