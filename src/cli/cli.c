#include "cli.h"

#include <sys/user.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cli_breakpoint.h"
#include "cli_regs.h"
#include "cli_step.h"

#include "../dbg/dbg.h"
#include "../dbg/dbg_breakpoint.h"
#include "../dbg/dbg_regs.h"
#include "../dbg/dbg_step.h"


static void cli_launch(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);

    if (input->argc == 0) {
        return;
    }

    const dbg_result_t result = dbg_launch(dbg, input->argv);
    const char* message = dbg_result_str(result);
    if (message) {
        fprintf(stderr, ERROR_FMT, message);
    }
}

static void cli_quit(dbg_t* dbg, const user_input_t* input) { 
    assert(dbg != NULL);
    assert(input != NULL);

    if (input->argc > 0) {
        fprintf(stderr, ERROR_FMT, "too many arguments"); 
        return;
    }

    const dbg_result_t result = dbg_quit(dbg);
    const char* message = dbg_result_str(result);
    if (message) {
        fprintf(stderr, ERROR_FMT, message);
    }
}

static void cli_continue(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);

    const dbg_result_t result = dbg_continue(dbg);
    const char* message = dbg_result_str(result);
    if (message) {
        fprintf(stderr, ERROR_FMT, message);
    }
}

static command_t commands[] = {
    { "launch", cli_launch },

    { "break", cli_set_breakpoint },

    { "quit", cli_quit },

    { "continue", cli_continue },

    { "si", cli_step_into },
    { "so", cli_step_out },
    { "sov", cli_step_over },

    { "rg", cli_get_regs },
    { "rs", cli_set_regs }
};

NODISCARD static user_input_t parse_line(char *line) {
    assert(line != NULL);

    int capacity = 4;

    user_input_t input = { 0 };
    input.argv = malloc(capacity * sizeof(char*));
    input.argc = 0;

    char* save = NULL;
    char* token = strtok_r(line, " ", &save);

    input.name = token;

    while ((token = strtok_r(NULL, " ", &save))) {
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
    assert(dbg != NULL);
    assert(line != NULL);

    const user_input_t input = parse_line(line);
    if (input.name == NULL) {
        return;
    }

    const unsigned long commands_size = sizeof(commands) / sizeof(commands[0]);
    for (unsigned long i = 0; i < commands_size; ++i) {
        if (strcmp(input.name, commands[i].name) == 0) {
            commands[i].handler(dbg, &input);
            free(input.argv);
            return;
        }
    }

    printf("unknown command: %s\n", input.name);

    free(input.argv);
}
