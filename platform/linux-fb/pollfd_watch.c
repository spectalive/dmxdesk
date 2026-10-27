#include "pollfd_watch.h"

void pollfd_watch(struct pollfd *fds, int *count, int fd) {
    fds[*count].fd = fd;
    fds[*count].events = POLLIN;
    fds[*count].revents = 0;
    (*count)++;
}
