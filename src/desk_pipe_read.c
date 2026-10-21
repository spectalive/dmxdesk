#include "desk_pipe_read.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "desk_pipe_parse.h"

int desk_pipe_read(struct desk_pipe_reader *reader, int fd,
                   struct touch_event *events, int cap) {
    if (!reader || fd < 0 || !events || cap <= 0 || reader->eof)
        return -1;
    if (reader->used < sizeof reader->data) {
        ssize_t got = read(fd, reader->data + reader->used,
                           sizeof reader->data - reader->used);
        if (got > 0)
            reader->used += (size_t)got;
        else if (got == 0)
            reader->eof = 1;
        else if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)
            reader->eof = 1;
    }
    int count = 0;
    while (count < cap) {
        char *end = memchr(reader->data, '\n', reader->used);
        if (!end)
            break;
        size_t len = (size_t)(end - reader->data);
        if (len < 128) {
            char line[128];
            memcpy(line, reader->data, len);
            line[len] = '\0';
            count += desk_pipe_parse(line, &events[count]);
        }
        size_t consumed = len + 1;
        memmove(reader->data, reader->data + consumed, reader->used - consumed);
        reader->used -= consumed;
    }
    if (reader->used == sizeof reader->data)
        reader->used = 0;
    return count ? count : reader->eof ? -1 : 0;
}
