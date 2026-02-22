#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "cli.h"

#define QBDBG_FMT       "qbdbg> "
#define QBDBG_HISTORY   ".qbdbg_history"

int main(void) {
    dbg_t dbg;

    using_history();
    read_history(QBDBG_HISTORY);

    while (1) {
        char *line = readline(QBDBG_FMT);
        if (!line) {
            break;
        }

        if (*line) {
            add_history(line);
        }

        dispatch_command(&dbg, line);
        free(line);
    }

    write_history(QBDBG_HISTORY);
    return 0;
}

