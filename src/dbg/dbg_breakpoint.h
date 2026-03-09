#pragma once

#ifndef DBG_BREAKPOINT_H
#define DBG_BREAKPOINT_H

#include "dbg.h"

dbg_result_t dbg_set_breakpoint(dbg_t* dbg, unsigned long addr);
dbg_result_t dbg_remove_breakpoint(dbg_t* dbg, unsigned long addr);

#endif // DBG_BREAKPOINT_H
