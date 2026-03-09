#pragma once

#ifndef DBG_STEP_H
#define DBG_STEP_H

#include "dbg.h"

dbg_result_t dbg_continue(dbg_t* dbg);
dbg_result_t dbg_single_step(dbg_t* dbg);

#endif // DBG_STEP_H
