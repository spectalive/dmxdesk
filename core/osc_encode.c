#include "osc_encode.h"

#include <stdint.h>
#include <string.h>

#include "osc_be32_write.h"
#include "osc_padded.h"

int osc_encode(const struct osc_msg *msg, char *buf, size_t cap) {
    size_t address_len = strnlen(msg->address, OSC_ADDRESS_MAX);
    if (msg->address[0] != '/' || address_len == OSC_ADDRESS_MAX)
        return -1;
    if (msg->type != 0 && msg->type != 'i' && msg->type != 'f')
        return -1;
    size_t tags_at = osc_padded(address_len);
    size_t tags_len = msg->type ? 2 : 1;
    size_t data_at = tags_at + osc_padded(tags_len);
    size_t total = data_at + (msg->type ? 4 : 0);
    if (total > cap)
        return -1;
    memset(buf, 0, total);
    memcpy(buf, msg->address, address_len);
    buf[tags_at] = ',';
    buf[tags_at + 1] = msg->type;
    uint32_t word = (uint32_t)msg->i;
    if (msg->type == 'f')
        memcpy(&word, &msg->f, sizeof word);
    if (msg->type)
        osc_be32_write(buf + data_at, word);
    return (int)total;
}
