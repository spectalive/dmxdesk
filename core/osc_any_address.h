// The wildcard address of family on port into out; its length.
#ifndef OSC_ANY_ADDRESS_H
#define OSC_ANY_ADDRESS_H

#include <sys/socket.h>

socklen_t osc_any_address(int family, int port, struct sockaddr_storage *out);

#endif
