#include "osc_decode.h"

#include <stdint.h>
#include <string.h>

#include "osc_be32_read.h"
#include "osc_padded.h"

int osc_decode(const char *buf, size_t len, struct osc_msg *out) {
    memset(out, 0, sizeof *out);
    if (len < 4 || len % 4 || buf[0] != '/')
        return -1;
    const char *address_end = memchr(buf, '\0', len);
    if (!address_end || address_end - buf >= OSC_ADDRESS_MAX)
        return -1;
    size_t address_len = (size_t)(address_end - buf);
    size_t tags_at = osc_padded(address_len);
    if (tags_at > len)
        return -1;
    memcpy(out->address, buf, address_len);
    if (tags_at == len)
        return 0;
    if (buf[tags_at] != ',')
        return -1;
    const char *tags_end = memchr(buf + tags_at, '\0', len - tags_at);
    if (!tags_end)
        return -1;
    size_t tags_len = (size_t)(tags_end - (buf + tags_at));
    size_t data_at = tags_at + osc_padded(tags_len);
    if (data_at > len)
        return -1;
    char type = tags_len >= 2 ? buf[tags_at + 1] : 0;
    if (type != 'i' && type != 'f')
        return 0;
    if (data_at + 4 > len)
        return -1;
    uint32_t word = osc_be32_read(buf + data_at);
    out->type = type;
    if (type == 'i')
        out->i = (int32_t)word;
    else
        memcpy(&out->f, &word, sizeof out->f);
    return 0;
}
