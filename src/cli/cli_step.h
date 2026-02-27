#pragma once

#ifndef CLI_STEP_H
#define CLI_STEP_H

#include "cli.h"

void cli_step_into(dbg_t* dbg, const user_input_t* input);
void cli_step_over(dbg_t* dbg, const user_input_t* input);
void cli_step_out(dbg_t* dbg, const user_input_t* input);

#endif // CLI_STEP_H
