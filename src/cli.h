#pragma once
#include "debugger.h"

typedef struct {
    const char* name;
    char** argv;
    int argc;
} user_input_t;

typedef struct {
    const char* name;
    void (*handler)(dbg_t* dbg, const user_input_t* input);
} command_t;

void dispatch_command(dbg_t* dbg, char* line); 
