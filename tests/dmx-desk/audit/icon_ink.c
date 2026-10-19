#include "api.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "icon.h"
// A tile's mark is ink like its words: record the icon's real alpha so the
// text checks see a caption that runs into it, then draw it for real.
void audit_icon(struct canvas *c, enum icon_id id, int x, int y, uint32_t colour) {
    if (id >= 0 && id < ICON_COUNT) {
        int x0=ICON_SIZE, y0=ICON_SIZE, x1=-1, y1=-1;
        for (int j=0;j<ICON_SIZE;j++) for (int i=0;i<ICON_SIZE;i++)
            if (ICON_ALPHA[id][j*ICON_SIZE+i]) {
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
                ink[j*iw+i]=ICON_ALPHA[id][(j+y0)*ICON_SIZE+i+x0]!=0;
            char label[24];
            snprintf(label,sizeof label,"icon %d",(int)id);
            audit_record((struct desk_rect){x+x0,y+y0,iw,ih},label,ink);
        }
    }
    icon_paint(c,id,x,y,colour);
}
