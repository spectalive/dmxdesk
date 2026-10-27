#include "hooks.h"
#undef icon_paint
#define canvas_blend audit_blend
#include "../../../core/icon.c"
