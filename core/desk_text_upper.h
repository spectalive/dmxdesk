// Capitals for ASCII and for the Latin-1 letters Spanish writes (U+00E0..
// U+00FE but the division sign, two bytes in UTF-8): "Dirección" reads
// "DIRECCIÓN", never "DIRECCIóN". A character cut by `cap` is not split.
#ifndef DESK_TEXT_UPPER_H
#define DESK_TEXT_UPPER_H

#include <stddef.h>

void desk_text_upper(char *dst, size_t cap, const char *src);

#endif
