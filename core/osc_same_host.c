#include "osc_same_host.h"

#include <netinet/in.h>
#include <string.h>

int osc_same_host(const struct sockaddr_storage *from, const struct sockaddr_storage *console) {
    if (from->ss_family != console->ss_family)
        return 0;
    if (from->ss_family == AF_INET)
        return ((const struct sockaddr_in *)from)->sin_addr.s_addr ==
               ((const struct sockaddr_in *)console)->sin_addr.s_addr;
    if (from->ss_family == AF_INET6)
        return !memcmp(&((const struct sockaddr_in6 *)from)->sin6_addr,
                       &((const struct sockaddr_in6 *)console)->sin6_addr, sizeof(struct in6_addr));
    return 0;
}
