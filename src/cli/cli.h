#pragma once

#ifndef CLI_H
#define CLI_H

#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../fwd.h"

#define QBDBG_FMT       "qbdbg> "
#define QBDBG_HISTORY   ".qbdbg_history"
#define ERROR_FMT       "error: %s\n"

typedef struct user_input_t {
    const char* name;
    char** argv;
    int argc;
} user_input_t;

typedef struct {
    const char* name;
    void (*handler)(dbg_t* dbg, const user_input_t* input);
} command_t;

void dispatch_command(dbg_t* dbg, char* line); 

#endif // CLI_H
