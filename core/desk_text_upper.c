#include "desk_text_upper.h"

void desk_text_upper(char *dst, size_t cap, const char *src) {
    size_t i = 0;
    for (; src[i] && i + 1 < cap; i++) {
        unsigned char b = (unsigned char)src[i], next = (unsigned char)src[i + 1];
        if (b == 0xC3 && next >= 0xA0 && next <= 0xBE && next != 0xB7) {
            if (i + 2 >= cap)
                break;
            dst[i] = (char)b;
            dst[++i] = (char)(next - 0x20);
            continue;
        }
        dst[i] = (b >= 'a' && b <= 'z') ? (char)(b - 32) : (char)b;
    }
    dst[i] = '\0';
}
