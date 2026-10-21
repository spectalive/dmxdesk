// A change in what runs moves words outside the control's own rectangle: the
// SALA AHORA hero and the bar's status word follow the room's state, and a
// naming panel's heading follows the tiles inside it. Ambient OFF also follows
// the rhythms' state. Joins those to the damage on the current view.
#ifndef DESK_DAMAGE_RUNNING_H
#define DESK_DAMAGE_RUNNING_H

#include "desk_model.h"

void desk_damage_running(struct desk_model *m, int control);

#endif
