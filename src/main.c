#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "cli.h"

int main(void) {
    dbg_t dbg;

    while (1) {
        char *line = readline("qbdbg> ");
        if (!line) {
            break;
        }

        if (*line) {
            add_history(line);
        }

        dispatch_command(&dbg, line);
        free(line);
    }

    return 0;
}

