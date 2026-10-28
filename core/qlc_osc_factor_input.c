#include "qlc_osc_factor_input.h"

int qlc_osc_factor_input(int factor) {
    static const int input[11] = { -1, -1, 6, 7, 8, 9, 4, 10, 11, 12, 13 };
    return factor >= 0 && factor <= 10 ? input[factor] : -1;
}
