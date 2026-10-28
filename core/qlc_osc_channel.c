#include "qlc_osc_channel.h"

uint16_t qlc_osc_channel(const char *path) {
    static const uint16_t table[16] = {
        0x0000, 0x1081, 0x2102, 0x3183, 0x4204, 0x5285, 0x6306, 0x7387,
        0x8408, 0x9489, 0xa50a, 0xb58b, 0xc60c, 0xd68d, 0xe70e, 0xf78f,
    };
    uint16_t crc = 0xffff;
    for (const unsigned char *p = (const unsigned char *)path; *p; p++) {
        unsigned c = *p;
        crc = (uint16_t)(((crc >> 4) & 0x0fff) ^ table[(crc ^ c) & 15]);
        c >>= 4;
        crc = (uint16_t)(((crc >> 4) & 0x0fff) ^ table[(crc ^ c) & 15]);
    }
    return (uint16_t)~crc;
}
