#include "api.h"
void audit_blend_round(struct canvas *c, int x, int y, int w, int h, int r, uint32_t col) {
    audit_record((struct desk_rect){x,y,w,h}, NULL, NULL);
    canvas_blend_round_rect(c,x,y,w,h,r,col);
}
