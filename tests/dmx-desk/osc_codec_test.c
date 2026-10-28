// SOURCES: osc_encode.c osc_decode.c osc_padded.c osc_be32_read.c osc_be32_write.c osc_msg_number.c osc_msg_format.c
// OSC on the wire (2026-10-28): the bytes a console expects for an int, a
// float and a bare address, the same message read back, and every malformed
// datagram refused rather than half read.
#include <assert.h>
#include <string.h>

#include "osc_decode.h"
#include "osc_encode.h"
#include "osc_msg_format.h"
#include "osc_msg_number.h"

static struct osc_msg msg(const char *address, char type, int i, float f) {
    struct osc_msg m;
    memset(&m, 0, sizeof m);
    strcpy(m.address, address);
    m.type = type;
    m.i = i;
    m.f = f;
    return m;
}

int main(void) {
    char buf[128];
    struct osc_msg m, back;

    // The exact bytes: address and type tag padded to four, big-endian data.
    m = msg("/a", 'f', 0, 1.0f);
    assert(osc_encode(&m, buf, sizeof buf) == 12);
    assert(!memcmp(buf, "/a\0\0,f\0\0\x3f\x80\0\0", 12));
    m = msg("/abcd", 'i', 700, 0);
    assert(osc_encode(&m, buf, sizeof buf) == 16);
    assert(!memcmp(buf, "/abcd\0\0\0,i\0\0\0\0\x02\xbc", 16));
    m = msg("/abc", 0, 0, 0);
    assert(osc_encode(&m, buf, sizeof buf) == 12);
    assert(!memcmp(buf, "/abc\0\0\0\0,\0\0\0", 12));

    // Round trips, negative ints and fractions included.
    m = msg("/vibra/12/ms", 'i', -42, 0);
    int n = osc_encode(&m, buf, sizeof buf);
    assert(osc_decode(buf, (size_t)n, &back) == 0);
    assert(!strcmp(back.address, "/vibra/12/ms") && back.type == 'i' && back.i == -42);
    assert(osc_msg_number(&back) == -42.0f);
    m = msg("/gm", 'f', 0, 0.5f);
    n = osc_encode(&m, buf, sizeof buf);
    assert(osc_decode(buf, (size_t)n, &back) == 0 && back.type == 'f' && back.f == 0.5f);
    assert(osc_msg_number(&back) == 0.5f);
    m = msg("/abc", 0, 0, 0);
    n = osc_encode(&m, buf, sizeof buf);
    assert(osc_decode(buf, (size_t)n, &back) == 0 && back.type == 0);
    assert(osc_msg_number(&back) == 0.0f);

    // Encoding refuses what is not a message, and a buffer too small.
    m = msg("vibra", 'i', 1, 0);
    assert(osc_encode(&m, buf, sizeof buf) == -1);
    m = msg("/a", 's', 0, 0);
    assert(osc_encode(&m, buf, sizeof buf) == -1);
    m = msg("/a", 'f', 0, 1.0f);
    assert(osc_encode(&m, buf, 11) == -1);
    memset(&m, 0, sizeof m);
    memset(m.address, 'a', sizeof m.address);
    m.address[0] = '/';
    assert(osc_encode(&m, buf, sizeof buf) == -1);

    // No type tag at all is OSC 1.0's leniency: an address, no number.
    assert(osc_decode("/old\0\0\0\0", 8, &back) == 0 && !strcmp(back.address, "/old") &&
           back.type == 0);
    // A first argument that is not a number reads as none.
    assert(osc_decode("/s\0\0,s\0\0hi\0\0", 12, &back) == 0 && back.type == 0 &&
           !strcmp(back.address, "/s"));

    // Malformed: not a multiple of four, no leading slash, a bundle, no NUL,
    // a tag without its comma, a tag running off the end, a missing argument.
    assert(osc_decode("/a\0\0,f\0\0\x3f\x80\0", 11, &back) == -1);
    assert(osc_decode("a\0\0\0,f\0\0\x3f\x80\0\0", 12, &back) == -1);
    assert(osc_decode("#bundle\0\0\0\0\0\0\0\0\x01", 16, &back) == -1);
    assert(osc_decode("/abc", 4, &back) == -1);
    assert(osc_decode("/a\0\0xf\0\0\x3f\x80\0\0", 12, &back) == -1);
    assert(osc_decode("/a\0\0,fff", 8, &back) == -1);
    assert(osc_decode("/a\0\0,f\0\0", 8, &back) == -1);
    assert(osc_decode("", 0, &back) == -1);
    char long_address[OSC_ADDRESS_MAX + 4];
    memset(long_address, 'a', sizeof long_address);
    long_address[0] = '/';
    long_address[OSC_ADDRESS_MAX] = '\0';
    assert(osc_decode(long_address, OSC_ADDRESS_MAX + 4, &back) == -1);

    // The log line.
    m = msg("/x/2", 'f', 0, 1.0f);
    assert(osc_msg_format(&m, buf, sizeof buf) == 6 && !strcmp(buf, "/x/2 1"));
    m = msg("/x/12/ms", 'i', 700, 0);
    assert(osc_msg_format(&m, buf, sizeof buf) > 0 && !strcmp(buf, "/x/12/ms 700"));
    assert(osc_msg_format(&m, buf, 4) == -1);
    return 0;
}
