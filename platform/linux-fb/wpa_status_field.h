// One `key=value` line out of a supplicant STATUS reply.
#ifndef WPA_STATUS_FIELD_H
#define WPA_STATUS_FIELD_H

#include <stddef.h>

void wpa_status_field(const char *reply, const char *key, char *out, size_t cap);

#endif
