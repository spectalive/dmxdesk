// Appends fd to a pollfd array, watched for input.
#ifndef POLLFD_WATCH_H
#define POLLFD_WATCH_H

#include <poll.h>

void pollfd_watch(struct pollfd *fds, int *count, int fd);

#endif
