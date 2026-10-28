#include "osc_any_address.h"

#include <netinet/in.h>
#include <stdint.h>
#include <string.h>

socklen_t osc_any_address(int family, int port, struct sockaddr_storage *out) {
    memset(out, 0, sizeof *out);
    if (family == AF_INET6) {
        struct sockaddr_in6 *a = (struct sockaddr_in6 *)out;
        a->sin6_family = AF_INET6;
        a->sin6_port = htons((uint16_t)port);
        a->sin6_addr = in6addr_any;
        return sizeof *a;
    }
    struct sockaddr_in *a = (struct sockaddr_in *)out;
    a->sin_family = AF_INET;
    a->sin_port = htons((uint16_t)port);
    a->sin_addr.s_addr = htonl(INADDR_ANY);
    return sizeof *a;
}
