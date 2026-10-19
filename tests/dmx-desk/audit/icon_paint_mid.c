#include "hooks.h"
#undef icon_paint_mid
#define canvas_blend audit_blend
#include "../../../src/icon_paint_mid.c"
