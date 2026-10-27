#ifndef DESK_PIPE_READ_H
#define DESK_PIPE_READ_H

#include <stddef.h>

#include "touch_event.h"

struct desk_pipe_reader {
    char data[4096];
    size_t used;
    int eof;
};

int desk_pipe_read(struct desk_pipe_reader *reader, int fd,
                   struct touch_event *events, int cap);

#endif
