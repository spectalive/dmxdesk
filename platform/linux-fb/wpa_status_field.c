#include "wpa_status_field.h"

#include <string.h>

void wpa_status_field(const char *reply, const char *key, char *out, size_t cap) {
    out[0] = '\0';
    size_t klen = strlen(key);
    for (const char *p = reply; p && *p; ) {
        const char *end = strchr(p, '\n');
        size_t len = end ? (size_t)(end - p) : strlen(p);
        if (len > klen && strncmp(p, key, klen) == 0 && p[klen] == '=') {
            size_t n = len - klen - 1;
            if (n >= cap)
                n = cap - 1;
            memcpy(out, p + klen + 1, n);
            out[n] = '\0';
            return;
        }
        p = end ? end + 1 : NULL;
    }
}
