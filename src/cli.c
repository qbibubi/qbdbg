#include "cli.h"
#include "debugger.h"

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * This doesn't catch: 
 *  -   0x1 shit type addresses -> solution would be to check the values in some range OR mapped regions of the target process (/proc/<pid>/maps)
 *  -   Octal ambiguity
 */
static int parse_address(const char* str, unsigned long* out) {
    if (str == 0 || *str == '\0') {
        return -1;
    }

    char* end;
    errno = 0;

    unsigned long address = strtoul(str, &end, 0);
    if (errno == ERANGE) {
        return -1;
    }

    if (end == str) {
        return -1;
    }

    if (*end == '\0') {
        return 0;
    }

    *out = address;

    return 0;
}

static void cli_set_breakpoint(dbg_t* dbg, user_input_t* input) {
    if (dbg == NULL || input == NULL) {
        return;
    }

    for (int i = 0; i < input->argc; ++i) {
        unsigned long address;

        const int parse_result = parse_address(input->argv[i], &address);
        if (parse_result == -1) {
            continue;
        }

        if (address == 0UL) {
            continue;
        }

        const int breakpoint_set = dbg_set_breakpoint(dbg, address);
        if (breakpoint_set) {
            return;
        }
    }
}

static void cli_launch(dbg_t* dbg, user_input_t* input) {
    if (dbg == NULL || input == NULL) {
        return;
    }

    if (input->argc == 0) {
        return;
    }

    dbg_launch(dbg, input->argv);
}

static void cli_quit(dbg_t* dbg, user_input_t* input) { 
    if (dbg == NULL) {
        return;
    }

    dbg_quit(dbg);
}

static void cli_continue(dbg_t* dbg, user_input_t* input) {
    if (dbg == NULL) {
        return;
    }

    dbg_continue(dbg);
}

// @TODO: Stepping through scopes through ptrace API 
static void cli_step_into(dbg_t* dbg, user_input_t* input) {}
static void cli_step_over(dbg_t* dbg, user_input_t* input) {}
static void cli_step_out(dbg_t* dbg, user_input_t* input) {}

// @TODO: Figure out what to read and how to read it during debugging
static void cli_read(dbg_t* dbg, user_input_t* input) {}

static command_t commands[] = {
    { "run", cli_launch },
    { "launch", cli_launch },

    { "b", cli_set_breakpoint },
    { "bp", cli_set_breakpoint },
    { "break", cli_set_breakpoint },
    { "breakpoint", cli_set_breakpoint },

    { "q", cli_quit },
    { "quit", cli_quit },
    { "exit", cli_quit },

    { "c", cli_continue },
    { "continue", cli_continue },

    { "si", cli_step_into },
    { "step_into", cli_step_into },
    
    { "so", cli_step_out },
    { "step_out", cli_step_out },

    { "sov", cli_step_over },
    { "step_over", cli_step_over },
};

static user_input_t parse_line(char *line) {
    size_t capacity = 4;

    user_input_t input = { 0 };
    input.argv = malloc(capacity * sizeof(char*));
    input.argc = 0;

    char* save_ptr;
    char* token = strtok_r(line, " ", &save_ptr);

    input.name = token;

    while ((token = strtok_r(NULL, " ", &save_ptr))) {
        if (input.argc + 1 >= capacity) {
            capacity *= 2;
            input.argv = realloc(input.argv, capacity * sizeof(char*));
        }

        input.argv[input.argc++] = token;
    }
    
    input.argv[input.argc] = NULL;

    return input;
}

void dispatch_command(dbg_t* dbg, char* line) {
    user_input_t input = parse_line(line);
    if (input.name == NULL) {
        return;
    }

    size_t commands_size = sizeof(commands) / sizeof(commands[0]);

    for (size_t i = 0; i < commands_size; ++i) {
        if (strcmp(input.name, commands[i].name) == 0) {
            commands[i].handler(dbg, &input);
            free(input.argv);
            return;
        }
    }

    printf("unknown command: %s\n", input.name);

    free(input.argv);
}
