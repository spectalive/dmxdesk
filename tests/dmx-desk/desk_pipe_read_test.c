// SOURCES: desk_pipe_read.c desk_pipe_parse.c
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

#include "desk_pipe_read.h"

int main(void) {
    int fd[2];
    assert(pipe(fd) == 0);
    assert(fcntl(fd[0], F_SETFL, O_NONBLOCK) == 0);
    struct desk_pipe_reader reader = {0};
    struct touch_event events[4];
    assert(write(fd[1], "D 0 12", 6) == 6);
    assert(desk_pipe_read(&reader, fd[0], events, 4) == 0);
    assert(write(fd[1], " 30 400\nU 0 12 30 430\n", 22) == 22);
    assert(desk_pipe_read(&reader, fd[0], events, 4) == 2);
    assert(events[0].kind == TOUCH_DOWN && events[1].kind == TOUCH_UP);
    assert(events[0].x == 12 && events[0].y == 30);
    assert(write(fd[1], "garbage\nC 1 0 0 500\n", 20) == 20);
    assert(desk_pipe_read(&reader, fd[0], events, 4) == 1);
    assert(events[0].kind == TOUCH_CANCEL && events[0].slot == 1);
    close(fd[1]);
    assert(desk_pipe_read(&reader, fd[0], events, 4) == -1);
    close(fd[0]);
    return 0;
}
