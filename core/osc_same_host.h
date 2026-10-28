// Whether a datagram came from the console's own host: the same family and
// address, whatever the port, since a console may answer from a port other
// than the one it listens on. Anything else on the network is not heard.
#ifndef OSC_SAME_HOST_H
#define OSC_SAME_HOST_H

#include <sys/socket.h>

int osc_same_host(const struct sockaddr_storage *from, const struct sockaddr_storage *console);

#endif
