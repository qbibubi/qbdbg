#pragma once
#include <sys/types.h>

#include "util.h"

#define MAX_BREAKPOINTS 128

typedef enum {
    DBG_OK,
    
    DBG_ERR_PTRACE,
    DBG_ERR_FORK,       // os_fork
    DBG_ERR_EXEC,       // os_exec
    DBG_ERR_ATTACH,     // os_attach
    DBG_ERR_WAIT,       // os_wait

    DBG_ERR_MEM_READ,   // os_read_mem 
    DBG_ERR_MEM_WRITE,  // os_write_mem
    DBG_ERR_REG_READ,   // os_get_regs
    DBG_ERR_REG_WRITE,  // os_set_regs

    DBG_ERR_NOT_RUNNING,
    DBG_ERR_ALREADY_RUNNING,
    DBG_ERR_NOT_STOPPED,

    DBG_ERR_BP_DUPLICATE,
    DBG_ERR_BP_NOT_FOUND,
    DBG_ERR_TABLE_FULL,
    DBG_ERR_BP_READ,        // os_read_mem (original byte)
    DBG_ERR_BP_WRITE,       // os_write_mem (failed writing interrupt)

    DBG_ERR_INVALID_ADDR,
    DBG_ERR_INVALID_ARG
} dbg_result_t;

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
    breakpoint_t breakpoints[MAX_BREAKPOINTS];
    int count;
} breakpoint_table_t;

typedef struct {
    pid_t pid;
    dbg_state_t state;
    breakpoint_table_t breakpoints;
    int running;
    int last_signal;
    int stepping_over_bp;
} dbg_t;

NODISCARD dbg_result_t dbg_init(dbg_t* dbg);
NODISCARD dbg_result_t dbg_quit(dbg_t* dbg);

NODISCARD dbg_result_t dbg_launch(dbg_t* dbg, char** argv);
NODISCARD dbg_result_t dbg_continue(dbg_t* dbg);
NODISCARD dbg_result_t dbg_single_step(dbg_t* dbg);

NODISCARD dbg_result_t dbg_set_breakpoint(dbg_t* dbg, unsigned long addr);
NODISCARD dbg_result_t dbg_remove_breakpoint(dbg_t* dbg, unsigned long addr);
