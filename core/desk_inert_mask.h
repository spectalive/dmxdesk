#ifndef DESK_INERT_MASK_H
#define DESK_INERT_MASK_H
#include "desk_model.h"
#include "desk_setup.h"
#include "desk_speed.h"
#include "desk_placement_target.h"
#include "desk_region_mark.h"
#include "desk_setup_inert_mask.h"
#include "desk_speed_inert_mask.h"
#include "desk_speed_paint.h"
#include "desk_tab.h"
#include "desk_tab_page.h"
#include "desk_view_family.h"
void desk_inert_mask(const struct desk_model *m, const struct desk_setup *s, const struct desk_speed *speed, unsigned char *mask);
#endif
