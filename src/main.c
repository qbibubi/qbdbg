#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "cli.h"

#define QBDBG_FMT       "qbdbg> "
#define QBDBG_HISTORY   ".qbdbg_history"

int main(void) {
    dbg_t dbg;
    const dbg_result_t result = dbg_init(&dbg);
    if (result != DBG_OK) {
        return 1; 
    }

    using_history();
    read_history(QBDBG_HISTORY);

    while (dbg.running) {
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

