#include "osc_resolve.h"

#include <netdb.h>
#include <stdio.h>
#include <string.h>

int osc_resolve(const char *host, int port, struct sockaddr_storage *dest, socklen_t *len,
                char *why, size_t cap) {
    if (!host[0]) {
        snprintf(why, cap, "no console address");
        return -1;
    }
    char service[16];
    snprintf(service, sizeof service, "%d", port);
    struct addrinfo hints = { .ai_family = AF_UNSPEC, .ai_socktype = SOCK_DGRAM };
    struct addrinfo *list = NULL;
    int rc = getaddrinfo(host, service, &hints, &list);
    if (rc != 0) {
        snprintf(why, cap, "%s: %s", host, gai_strerror(rc));
        return -1;
    }
    memcpy(dest, list->ai_addr, list->ai_addrlen);
    *len = list->ai_addrlen;
    freeaddrinfo(list);
    return 0;
}
