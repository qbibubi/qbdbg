#pragma once

#ifndef CLI_REGS_H
#define CLI_REGS_H

#include "cli.h"

void cli_get_regs(dbg_t* dbg, const user_input_t* input); 
void cli_set_regs(dbg_t* dbg, const user_input_t* input);

#endif // CLI_REGS_H
