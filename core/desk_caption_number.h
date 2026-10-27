// The number a caption ends in: the trailing run of words that are all
// digits, joined by "y" ("Gobo 14" -> "14", "1 y 3" -> "1 y 3"), which
// starts and ends on digits. What comes before it is the caption's stem
// ("Gobo"), empty when the caption is the number alone.
#ifndef DESK_CAPTION_NUMBER_H
#define DESK_CAPTION_NUMBER_H

// Returns where the number starts inside `caption`, or NULL when it ends in none.
const char *desk_caption_number(const char *caption);

#endif
