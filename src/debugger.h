#pragma once
#include <sys/types.h>

typedef enum {
    DBG_STOPPED,
    DBG_RUNNING,
    DBG_STEPPING,
    DBG_EXITED,
} dbg_state_t;

typedef struct {
    unsigned long address;
    unsigned long original_byte;
    int enabled;
} breakpoint_t;

typedef struct {
    breakpoint_t breakpoints[128];
    int count;
} breakpoint_table_t;

typedef struct {
    pid_t pid;
    dbg_state_t state;
    breakpoint_table_t breakpoints;
    int last_signal;
    int stepping_over_bp;
} dbg_t;

void dbg_init(dbg_t *dbg);
void dbg_launch(dbg_t *dbg, const char *path);
void dbg_continue(dbg_t *dbg);
void dbg_single_step(dbg_t *dbg);
int  dbg_set_breakpoint(dbg_t *dbg, unsigned long addr);
void dbg_remove_breakpoint(dbg_t *dbg, unsigned long addr);

