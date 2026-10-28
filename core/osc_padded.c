#include "osc_padded.h"

size_t osc_padded(size_t n) {
    return (n + 4) & ~(size_t)3;
}
