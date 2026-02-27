#include "dbg_step.h"

#include "../os/os.h"

dbg_result_t dbg_continue(dbg_t *dbg) {
    assert(dbg != NULL);

    if (os_continue(dbg->pid, 0) != OS_OK) {
        return DBG_ERR_CONTINUE;
    }

    dbg->state = DBG_RUNNING;

    return DBG_OK;
}

dbg_result_t dbg_single_step(dbg_t *dbg) {
    assert(dbg != NULL);

    if (os_single_step(dbg->pid) != OS_OK) {
        return DBG_ERR_SINGLE_STEP;
    }

    dbg->state = DBG_STEPPING;

    return DBG_OK;
}

