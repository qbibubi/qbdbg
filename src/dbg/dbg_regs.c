#include "dbg_regs.h"

#include "../os/os.h"

static void regs_from_native(dbg_regs_t* out, const struct user_regs_struct* native) {
    out->r15    = native->r15;
    out->r14    = native->r14;
    out->r13    = native->r13;
    out->r12    = native->r12;
    out->rbp    = native->rbp;
    out->rbx    = native->rbx;
    out->r11    = native->r11;
    out->r10    = native->r10;
    out->r9     = native->r9;
    out->r8     = native->r8;
    out->rax    = native->rax;
    out->rcx    = native->rcx;
    out->rdx    = native->rdx;
    out->rsi    = native->rsi;
    out->rdi    = native->rdi;
    out->rip    = native->rip;
    out->rsp    = native->rsp;
    out->cs     = native->cs;
    out->eflags = native->eflags;
    out->ss     = native->ss;
    out->ds     = native->ds;
    out->es     = native->es;
    out->fs     = native->fs;
    out->gs     = native->gs;
}

static void regs_to_native(struct user_regs_struct* native, const dbg_regs_t* regs) {
   native->r15 = regs->r15;    
   native->r14 = regs->r14;    
   native->r13 = regs->r13;    
   native->r12 = regs->r12;    
   native->rbp = regs->rbp;    
   native->rbx = regs->rbx;    
   native->r11 = regs->r11;    
   native->r10 = regs->r10;    
   native->r9 = regs->r9;    
   native->r8 = regs->r8;     
   native->rax = regs->rax;    
   native->rcx = regs->rcx;    
   native->rdx = regs->rdx;    
   native->rsi = regs->rsi;    
   native->rdi = regs->rdi;    
   native->rip = regs->rip;    
   native->rsp = regs->rsp;    
   native->cs = regs->cs;     
   native->eflags = regs->eflags;
   native->ss = regs->ss;     
   native->ds = regs->ds;     
   native->es = regs->es;     
   native->fs = regs->fs;     
   native->gs = regs->gs;     
}

dbg_result_t dbg_get_regs(dbg_t* dbg, dbg_regs_t* out_regs) {
    assert(dbg != NULL);
    assert(out_regs != NULL);

    struct user_regs_struct native;

    if (os_get_regs(dbg->pid, &native) == OS_ERR_GET_REGS) {
        return DBG_ERR_GET_REGS;
    }

    regs_from_native(out_regs, &native);

    return DBG_OK;
}

dbg_result_t dbg_set_regs(dbg_t* dbg, dbg_regs_t* regs) {
    assert(dbg != NULL);
    assert(regs != NULL);

    struct user_regs_struct native;
    if (os_get_regs(dbg->pid, &native) == OS_ERR_GET_REGS) {
        return DBG_ERR_GET_REGS;
    }

    regs_to_native(&native, regs);
    
    if (os_set_regs(dbg->pid, &native) == OS_ERR_SET_REGS) {
        return DBG_ERR_SET_REGS;
    }

    return DBG_OK;
}
