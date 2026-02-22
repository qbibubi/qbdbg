#include "debugger.h"
#include <memory.h>

#include "os.h"

dbg_result_t dbg_init(dbg_t *dbg) {
    dbg->running = 1;
    dbg->state = DBG_STOPPED;
    dbg->pid = 0;
    dbg->last_signal = 0;
    dbg->stepping_over_bp = 0;

    memset(&dbg->breakpoints, 0, sizeof(breakpoint_table_t));

    return DBG_OK;
}

dbg_result_t dbg_launch(dbg_t *dbg, char** argv) {
    if (dbg->state != DBG_STOPPED || dbg->pid != 0) {
        return DBG_ERR_ALREADY_RUNNING;
    }

    pid_t pid;
    os_result_t os_result = os_fork(&pid);
    if (os_result != OS_OK) {
        return DBG_ERR_FORK;
    }
    
    const int is_child_process = (pid == 0);
    if (is_child_process) {
        os_result = os_traceme();
        if (os_result != OS_OK) {
            return DBG_ERR_PTRACE;
        }

        os_result = os_exec(argv[0], argv);
        if (os_result == OS_ERR_EXEC) {
             return DBG_ERR_EXEC; 
        }

        _exit(1);
    }

    dbg->pid = pid;
    dbg->state = DBG_STOPPED;

    int status;
    os_result = os_wait(pid, &status);
    if (os_result != OS_OK) {
        return DBG_ERR_WAIT; 
    }
    
    return DBG_OK;
}

dbg_result_t dbg_quit(dbg_t *dbg) {
    const os_result_t result = os_detach(dbg->pid);
    if (result != OS_OK) {
        return OS_ERR_DETACH;
    }
    
    dbg->running = 0;

    return DBG_OK;
}

dbg_result_t dbg_continue(dbg_t *dbg) {
    
    return DBG_OK;
}

dbg_result_t dbg_single_step(dbg_t *dbg) {

    return DBG_OK;
}

dbg_result_t dbg_set_breakpoint(dbg_t *dbg, unsigned long addr) {
    // Check if the breakpoint already exists so we don't store same breakpoint twice

    return DBG_OK;
}

dbg_result_t dbg_remove_breakpoint(dbg_t *dbg, unsigned long addr) {
    // Find the breakpoint in breakpoints list

    return DBG_OK;
}

