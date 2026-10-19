#include "api.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "icon_paint_hero.h"
// The hero's mark is ink beside the state's name: record its real alpha so
// the text checks see a name that runs into it, then draw it.
int audit_icon_hero(struct canvas *c, enum icon_id id, int x, int y, uint32_t colour) {
    if (id >= 0 && id < ICON_COUNT && ICON_HERO_OF[id] >= 0) {
        const uint8_t *a = ICON_HERO_ALPHA[ICON_HERO_OF[id]];
        int x0=ICON_HERO_SIZE, y0=ICON_HERO_SIZE, x1=-1, y1=-1;
        for (int j=0;j<ICON_HERO_SIZE;j++) for (int i=0;i<ICON_HERO_SIZE;i++)
            if (a[j*ICON_HERO_SIZE+i]) {
                if(i<x0)x0=i;
                if(i>x1)x1=i;
                if(j<y0)y0=j;
                if(j>y1)y1=j;
            }
        if (x1>=x0) {
            int iw=x1-x0+1, ih=y1-y0+1;
            unsigned char *ink=calloc((size_t)iw*ih,1);
            assert(ink);
            for(int j=0;j<ih;j++)for(int i=0;i<iw;i++)
                ink[j*iw+i]=a[(j+y0)*ICON_HERO_SIZE+i+x0]!=0;
            char label[24];
            snprintf(label,sizeof label,"icon72 %d",(int)id);
            audit_record((struct desk_rect){x+x0,y+y0,iw,ih},label,ink);
        }
    }
    return icon_paint_hero(c,id,x,y,colour);
}
