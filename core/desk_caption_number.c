#include "desk_caption_number.h"

#include <string.h>

const char *desk_caption_number(const char *caption) {
    if (!caption)
        return NULL;
    // Walk the words from the end; `start` is the first word of the run so
    // far that is digits, so a joining "y" is only kept once a number precedes it.
    const char *start = NULL, *end = caption + strlen(caption);
    while (end > caption && end[-1] == ' ')
        end--;
    while (end > caption) {
        const char *word = end;
        while (word > caption && word[-1] != ' ')
            word--;
        size_t len = (size_t)(end - word);
        int digits = len > 0;
        for (size_t i = 0; i < len; i++)
            if (word[i] < '0' || word[i] > '9')
                digits = 0;
        if (digits)
            start = word;
        else if (!start || len != 1 || word[0] != 'y')
            break;
        end = word;
        while (end > caption && end[-1] == ' ')
            end--;
    }
    return start;
}
