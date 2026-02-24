#include "debugger.h"
#include <memory.h>
#include <sys/ptrace.h>

#include "os.h"

dbg_result_t dbg_init(dbg_t *dbg) {
    assert(dbg != NULL);

    dbg->running = 1;
    dbg->state = DBG_STOPPED;
    dbg->pid = 0;
    dbg->last_signal = 0;
    dbg->stepping_over_bp = 0;

    memset(&dbg->breakpoints, 0, sizeof(breakpoint_table_t));

    return DBG_OK;
}

dbg_result_t dbg_launch(dbg_t *dbg, char** argv) {
    assert(dbg != NULL);

    if (dbg->state != DBG_STOPPED || dbg->pid != 0) {
        return DBG_ERR_ALREADY_RUNNING;
    }

    pid_t pid;
    if (os_fork(&pid) != OS_OK) {
        return DBG_ERR_FORK;
    }
    
    if (pid == 0) {
        if (os_traceme() != OS_OK) {
            _exit(1);
        }

        if (os_exec(argv[0], argv) == OS_ERR_EXEC) {
             _exit(1); 
        }
    }

    dbg->pid = pid;
    dbg->state = DBG_STOPPED;

    int status;
    if (os_wait(pid, &status) != OS_OK) {
        return DBG_ERR_WAIT; 
    }
    
    return DBG_OK;
}

dbg_result_t dbg_quit(dbg_t *dbg) {
    assert(dbg != NULL);

    if (os_detach(dbg->pid) != OS_OK) {
        return DBG_ERR_ATTACH;
    }
    
    dbg->running = 0;

    return DBG_OK;
}

dbg_result_t dbg_continue(dbg_t *dbg) {
    assert(dbg != NULL);

    if (os_continue(dbg->pid, 0) != OS_OK) {
        return DBG_ERR_CONTINUE;
    }

    return DBG_OK;
}

dbg_result_t dbg_single_step(dbg_t *dbg) {
    assert(dbg != NULL);

    if (os_single_step(dbg->pid) != OS_OK) {
        return DBG_ERR_SINGLE_STEP;
    }

    return DBG_OK;
}

dbg_result_t dbg_set_breakpoint(dbg_t *dbg, unsigned long addr) {
    assert(dbg != NULL);

    breakpoint_t bp;
    bp.address = addr;
    bp.enabled = 1;

    if (os_read_mem(dbg->pid, addr, &bp.original_byte) == OS_ERR_READ_MEM) {
        return DBG_ERR_MEM_READ;
    }

    if (dbg->breakpoints.count == 0) {
        dbg->breakpoints.table[0] = bp;
        dbg->breakpoints.count++;
        return DBG_OK;
    }

    int breakpoint_found = 0;

    for (int i = 0; i < dbg->breakpoints.count; ++i) {
        if (dbg->breakpoints.table[i].address != 0) {
            continue;
        }

        if (dbg->breakpoints.table[i].address == addr) {
            breakpoint_found = 1;
            break;
        }

        if (os_write_mem(dbg->pid, addr, INTERRUPT_BYTE) == OS_ERR_WRITE_MEM) {
            return DBG_ERR_MEM_WRITE;
        }

        dbg->breakpoints.table[i] = bp;

        return DBG_OK;
    }

    if (breakpoint_found) {
        return DBG_ERR_BP_DUPLICATE;
    }

    return DBG_OK;
}

dbg_result_t dbg_remove_breakpoint(dbg_t *dbg, unsigned long addr) {
    assert(dbg != NULL);

    if (dbg->breakpoints.count == 0) {
        return DBG_ERR_BP_NOT_FOUND;
    }

    for (int i = 0; i < dbg->breakpoints.count; ++i) {
        if (dbg->breakpoints.table[i].address != addr) {
            continue;
        }

        if (os_write_mem(dbg->pid, addr, dbg->breakpoints.table->original_byte) == OS_ERR_WRITE_MEM) {
            return DBG_ERR_MEM_WRITE;
        }

        memset(&dbg->breakpoints.table[i], 0, sizeof(breakpoint_t));

        return DBG_OK;
    }

    return DBG_ERR_BP_NOT_FOUND;
}

