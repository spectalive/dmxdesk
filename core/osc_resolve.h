// The console's UDP address into dest. 0, or -1 with why in why[].
#ifndef OSC_RESOLVE_H
#define OSC_RESOLVE_H

#include <stddef.h>
#include <sys/socket.h>

int osc_resolve(const char *host, int port, struct sockaddr_storage *dest, socklen_t *len,
                char *why, size_t cap);

#endif
