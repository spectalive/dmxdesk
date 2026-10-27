// Whether the bar would draw two status readings differently.
#ifndef DESK_STATUS_CHANGED_H
#define DESK_STATUS_CHANGED_H

#include "status.h"

int desk_status_changed(const struct status *a, const struct status *b);

#endif
