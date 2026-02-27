#include "dbg_breakpoint.h"

#include "../os/os.h"

#include <memory.h>

dbg_result_t dbg_set_breakpoint(dbg_t *dbg, unsigned long addr) {
    assert(dbg != NULL);

    if (addr == 0UL) {
        return DBG_ERR_INVALID_ADDR;
    }

    if (dbg->breakpoints.count >= MAX_BREAKPOINTS) {
        return DBG_ERR_TABLE_FULL;
    }

    breakpoint_t bp;
    bp.address = addr;
    bp.enabled = 1;

    if (os_read_mem(dbg->pid, addr, &bp.original_byte) == OS_ERR_READ_MEM) {
        return DBG_ERR_MEM_READ;
    }

    if (dbg->breakpoints.count == 0) {
        dbg->breakpoints.table[0] = bp;
        dbg->breakpoints.count++;

        if (os_write_mem(dbg->pid, addr, INTERRUPT_BYTE) == OS_ERR_WRITE_MEM) {
            return DBG_ERR_MEM_WRITE;
        }

        return DBG_OK;
    }

    int first_free = -1;

    for (int i = 0; i < dbg->breakpoints.count; ++i) {
        if (dbg->breakpoints.table[i].address == 0 && first_free == -1) {
            first_free = i;
        }
        
        if (dbg->breakpoints.table[i].address == addr) {
            return DBG_ERR_BP_DUPLICATE;
        }
    }

    if (first_free == -1) {
        first_free = dbg->breakpoints.count;
    }

    if (os_write_mem(dbg->pid, addr, INTERRUPT_BYTE) == OS_ERR_WRITE_MEM) {
        return DBG_ERR_MEM_WRITE;
    }

    dbg->breakpoints.table[first_free] = bp;
    dbg->breakpoints.count++;

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

        if (os_write_mem(dbg->pid, addr, dbg->breakpoints.table[i].original_byte) == OS_ERR_WRITE_MEM) {
            return DBG_ERR_MEM_WRITE;
        }

        memset(&dbg->breakpoints.table[i], 0, sizeof(breakpoint_t));

        dbg->breakpoints.count--;

        return DBG_OK;
    }

    return DBG_ERR_BP_NOT_FOUND;
}
