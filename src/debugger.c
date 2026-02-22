#include "debugger.h"

#include "os.h"

void dbg_init(dbg_t *dbg) {
    dbg->running = 1;
    dbg->state = DBG_STOPPED;
}

void dbg_launch(dbg_t *dbg, char** argv) {


}

void dbg_quit(dbg_t *dbg) {
    dbg->running = 0;
}

void dbg_continue(dbg_t *dbg) {
}

void dbg_single_step(dbg_t *dbg) {
}

int dbg_set_breakpoint(dbg_t *dbg, unsigned long addr) {
    // Check if the breakpoint already exists so we don't store same breakpoint twice

    return 0;
}

void dbg_remove_breakpoint(dbg_t *dbg, unsigned long addr) {
}

