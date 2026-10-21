#include "api.h"
#include "desk_pager_hit.h"
#include "desk_speed_layout.h"
#include "desk_speed_paint.h"
#include "desk_speed_target_rect.h"
#include "desk_setup_layout.h"
#include "desk_tab.h"
#include "desk_tab_page.h"
#include "desk_tab_title.h"
#include "desk_view_family.h"

void audit_inventory(struct audit_context *a) {
    a->elements=0;
    struct desk_model *m=&a->model;
    struct desk_rect r=desk_view_gear();
    audit_add(a,A_GEAR,0,0,1,"Gear",r,r,r);
    r=desk_view_lock_target();
    audit_add(a,A_LOCK,0,0,1,"Lock",r,r,r);
    // Painted tabs remain inventoried, but setup explicitly disables navigation.
    for(int i=0;i<DESK_TABS;i++) {
        if(desk_tab_page(m,i)<0)continue;
        r=desk_view_tab(i);
        audit_add(a,A_TAB,i,0,!m->setup_open,desk_tab_title((enum desk_tab)i),r,r,r);
    }
    for(int i=0;i<m->layout.placements;i++) {
        const struct desk_placement *p=&m->layout.placement[i];
        if(p->page!=m->page || p->bank!=m->bank)continue;
        if(a->setup.open && p->x<SETUP_SHEET_W)continue;
        if(p->control<0 || p->control>=m->count) {
            audit_note(a,0,"backing","placement %d references invalid control %d",i,p->control);
            continue;
        }
        const struct desk_control *c=&m->control[p->control];
        r=(struct desk_rect){p->x,p->y,p->w,p->h};
        if(c->kind==DESK_TEMPO)
            desk_speed_tempo_tap_rect(p->x,p->y,p->w,p->h,&r.x,&r.y,&r.w,&r.h);
        audit_add(a,A_CONTROL,p->control,0,c->enabled,c->label,r,r,r);
        if(c->kind==DESK_TEMPO && !a->setup.open)
            for(int step=SPEED_T_BPM_DOWN;step<=SPEED_T_BPM_UP;step++) {
                desk_speed_tempo_step_rect(p->x,p->y,p->w,p->h,(enum speed_target)step,
                                           &r.x,&r.y,&r.w,&r.h);
                audit_add(a,A_SPEED,step,0,desk_speed_target_enabled(&a->speed,0,(enum speed_target)step),
                          step==SPEED_T_BPM_DOWN?"Tempo Show -1 BPM":"Tempo Show +1 BPM",r,r,r);
            }
    }
    if(a->setup.open) { audit_setup_inventory(a); return; }
    // EFECTOS' families, on a family's page only; setup hides them.
    if(m->layout.tab[m->page]==DESK_TAB_EFECTOS)for(int i=0;i<m->layout.families;i++) {
        r=desk_view_family(i,m->layout.families);
        audit_add(a,A_FAMILY,i,0,1,m->layout.title[m->layout.family_page[i]],r,r,r);
    }
    if(m->layout.banks[m->page]>1)for(int i=0;i<m->layout.banks[m->page];i++) {
        char label[128];
        snprintf(label,sizeof label,"Bank %d",i+1);
        r=desk_view_pager(i,&m->layout,m->page);
        // The production helper explicitly assigns all glass below the pager
        // to it. Never infer this allowance from observed successful probes.
        struct desk_rect hit=desk_pager_hit(i,&m->layout,m->page);
        audit_add(a,A_BANK,i,0,1,label,r,hit,hit);
    }
    if(m->page==m->layout.speed_page) {
        for(int i=0;i<a->speed.dials && i<2;i++)for(int t=SPEED_T_TAP;t<=SPEED_T_DOUBLE;t++) {
            const char *label="Tap";
            switch(t) {
            case SPEED_T_BPM_DOWN:label="-1 BPM";break;
            case SPEED_T_BPM_UP:label="+1 BPM";break;
            case SPEED_T_FACTOR_ONE:label="Time x1";break;
            case SPEED_T_HALF:label="Slower x2";break;
            case SPEED_T_DOUBLE:label="Faster x2";break;
            default:break;
            }
            r=desk_speed_target_rect(i,(enum speed_target)t);
            char name[128];snprintf(name,sizeof name,"%s / %s",a->speed.dial[i].caption,label);
            audit_add(a,A_SPEED,t,i,desk_speed_target_enabled(&a->speed,i,t),name,r,r,r);
        }
        if(a->speed.dials>=2) {
            r=desk_speed_target_rect(-1,SPEED_T_BOTH);
            audit_add(a,A_SPEED,SPEED_T_BOTH,-1,desk_speed_target_enabled(&a->speed,0,SPEED_T_BOTH),"Tap both",r,r,r);
        }
    }
}
