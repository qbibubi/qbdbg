#pragma once

#ifndef DBG_REGS_H
#define DBG_REGS_H

#include "dbg.h"

NODISCARD dbg_result_t dbg_get_regs(dbg_t* dbg, dbg_regs_t*  out_regs);
NODISCARD dbg_result_t dbg_set_regs(dbg_t* dbg, dbg_regs_t* regs);

#endif // DBG_REGS_H
