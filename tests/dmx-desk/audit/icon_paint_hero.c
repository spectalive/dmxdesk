#include "hooks.h"
#undef icon_paint_hero
#define canvas_blend audit_blend
#include "../../../core/icon_paint_hero.c"
