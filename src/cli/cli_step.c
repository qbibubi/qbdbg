#include "cli_step.h"

#include "../dbg/dbg_step.h"

void cli_step_into(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);
}

void cli_step_over(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);

    const dbg_result_t result = dbg_single_step(dbg);
    const char* message = dbg_result_str(result);
    if (message) {
        fprintf(stderr, ERROR_FMT, message);
    }
}

void cli_step_out(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);

    // const dbg_result_t result = dbg_step_out(dbg, input);
    // const char* message = dbg_result_str(result);
    // if (message) {
    //     fprintf(stderr, ERROR_FMT, message);
    // }
}
