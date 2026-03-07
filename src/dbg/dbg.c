#include "dbg.h"

#include "../os/os.h"

#include <memory.h>
#include <stdlib.h>

dbg_result_t dbg_init(dbg_t *dbg) {
    assert(dbg != NULL);

    dbg->running = 1;
    dbg->state = DBG_STOPPED;
    dbg->pid = 0;
    dbg->last_signal = 0;
    dbg->exit_code = 0;
    dbg->stepping_over_bp = 0;

    memset(&dbg->breakpoints, 0, sizeof(breakpoint_table_t));

    return DBG_OK;
}

dbg_result_t dbg_quit(dbg_t *dbg) {
    assert(dbg != NULL);

    if (dbg->pid == 0) {
        dbg->running = 0;
        return DBG_OK;
    }

    if (os_detach(dbg->pid) != OS_OK) {
        return DBG_ERR_ATTACH;
    }
    
    dbg->running = 0;

    return DBG_OK;
}

dbg_result_t dbg_launch(dbg_t *dbg, char** argv) {
    assert(dbg != NULL);
    assert(argv != NULL);

    if (dbg->state != DBG_STOPPED || dbg->pid != 0) {
        return DBG_ERR_ALREADY_RUNNING;
    }

    pid_t pid;
    if (os_fork(&pid) != OS_OK) {
        return DBG_ERR_FORK;
    }
    
    const int is_child = (pid == 0);
    if (is_child) {
        if (os_traceme() != OS_OK) {
            _exit(1);
        }

        if (os_exec(argv[0], argv) == OS_ERR_EXEC) {
             _exit(1); 
        }
    }

    const int is_parent = (pid > 0);
    if (is_parent) {
        dbg->pid = pid;
        dbg->state = DBG_STOPPED;

        if (dbg_wait(dbg, pid) != DBG_OK) {
            return DBG_ERR_WAIT;
        }
    }
    
    return DBG_OK;
}

dbg_result_t dbg_wait(dbg_t* dbg, const pid_t pid) { 
    assert(dbg != NULL);

    int status;

    if (os_wait(pid, &status) != OS_OK) {
        return DBG_ERR_WAIT;
    }

    if (WIFEXITED(status)) {
        dbg->exit_code = WEXITSTATUS(status);
        dbg->state = DBG_EXITED;
        dbg->pid = 0;
    }
    else if (WIFSTOPPED(status)) {
        dbg->last_signal = WSTOPSIG(status);
        dbg->state = DBG_STOPPED;
    } 
    else if (WIFSIGNALED(status)) {
        dbg->last_signal = WTERMSIG(status);
        dbg->state = DBG_EXITED;
        dbg->pid = 0;
    } 
    else if (WIFCONTINUED(status)) {
        dbg->state = DBG_RUNNING;
    }
    
    return DBG_OK;
}

const char* dbg_result_str(dbg_result_t result) {
    switch (result) {
        case DBG_OK:                  return NULL; 

        case DBG_ERR_PTRACE:          return "ptrace operation failed";
        case DBG_ERR_FORK:            return "failed to fork child process";
        case DBG_ERR_EXEC:            return "failed to execute target";
        case DBG_ERR_ATTACH:          return "failed to attach to process";
        case DBG_ERR_DETACH:          return "failed to detach from process";
        case DBG_ERR_WAIT:            return "failed to wait for child";

        case DBG_ERR_CONTINUE:        return "failed to continue execution";
        case DBG_ERR_SINGLE_STEP:     return "failed to single step";
        case DBG_ERR_NOT_RUNNING:     return "no process is running";
        case DBG_ERR_ALREADY_RUNNING: return "process is already running";
        case DBG_ERR_NOT_STOPPED:     return "process is not stopped";

        case DBG_ERR_READ_MEM:
        case DBG_ERR_MEM_READ:        return "failed to read process memory";

        case DBG_ERR_WRITE_MEM:
        case DBG_ERR_MEM_WRITE:       return "failed to write process memory";

        case DBG_ERR_GET_REGS:
        case DBG_ERR_REG_READ:        return "failed to read process registers";

        case DBG_ERR_SET_REGS:
        case DBG_ERR_REG_WRITE:       return "failed to write process registers";

        case DBG_ERR_BP_DUPLICATE:    return "breakpoint already set at that address";
        case DBG_ERR_BP_NOT_FOUND:    return "no breakpoint at that address";
        case DBG_ERR_TABLE_FULL:      return "breakpoint table is full";
        case DBG_ERR_BP_READ:         return "failed to read original byte for breakpoint";
        case DBG_ERR_BP_WRITE:        return "failed to write interrupt instruction for breakpoint";

        case DBG_ERR_INVALID_ADDR:    return "invalid memory address";
        case DBG_ERR_INVALID_ARG:     return "invalid argument provided";

        default:                      return "unknown error";
    }
}
