// The desk's loop, the same on every machine: wait for a device or the
// master, take the power key and the touches, let the platform's services
// move, step the master, settle the models, read the bar, hand the frame
// over. Runs until the touch source ends, the platform is asked to stop or
// its frame says so.
#ifndef DESK_RUN_H
#define DESK_RUN_H

#include "desk_app.h"
#include "desk_platform.h"

void desk_run(struct desk_app *app, const struct desk_platform *p);

#endif
