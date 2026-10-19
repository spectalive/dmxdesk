#include "hooks.h"
#undef icon_paint_large
#define canvas_blend audit_blend
#include "../../../src/icon_paint_large.c"
