#include "cli_regs.h"

#include "../dbg/dbg_regs.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define REGISTER_FMT "\t%-8s 0x%016lx\n" 
#define REGISTER_ENTRY(name) { #name, offsetof(dbg_regs_t, name) }

typedef struct {
    const char* name;
    unsigned long offset;
} dbg_reg_t;

static const dbg_reg_t registers[] = {
    REGISTER_ENTRY(r15),
    REGISTER_ENTRY(r14),
    REGISTER_ENTRY(r13),
    REGISTER_ENTRY(r12),
    REGISTER_ENTRY(rbp),
    REGISTER_ENTRY(rbx),
    REGISTER_ENTRY(r11),
    REGISTER_ENTRY(r10),
    REGISTER_ENTRY(r9),
    REGISTER_ENTRY(r8),
    REGISTER_ENTRY(rax),
    REGISTER_ENTRY(rcx),
    REGISTER_ENTRY(rdx),
    REGISTER_ENTRY(rsi),
    REGISTER_ENTRY(rdi),
    REGISTER_ENTRY(rip),
    REGISTER_ENTRY(rsp),
    REGISTER_ENTRY(cs),
    REGISTER_ENTRY(eflags),
    REGISTER_ENTRY(ss),
    REGISTER_ENTRY(ds),
    REGISTER_ENTRY(es),
    REGISTER_ENTRY(fs),
    REGISTER_ENTRY(gs),
};

void cli_get_regs(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);

    dbg_regs_t regs;

    const dbg_result_t result = dbg_get_regs(dbg, &regs); 
    const char* message = dbg_result_str(result);
    if (result != DBG_OK) {
        fprintf(stderr, ERROR_FMT, message); 
        return;
    }

    const unsigned long registers_size = sizeof(registers) / sizeof(registers[0]);

    if (input->argc == 0) {
        for (unsigned long i = 0; i < registers_size; ++i) {
            const char* name = registers[i].name;
            const uint64_t value = *(uint64_t*)((uint8_t*)&regs + registers[i].offset);

            fprintf(stdout, REGISTER_FMT, name, value);
        }

        return;
    }

    for (int i = 0; i < input->argc; ++i) {
        const char* name = input->argv[i];
        if (!name) {
            continue;
        }

        int found = 0;

        for (unsigned long  j = 0; j < registers_size; ++j) {
            if (strcmp(name, registers[j].name) == 0) {
                const uint64_t value = *(uint64_t*)((uint8_t*)&regs + registers[j].offset);

                fprintf(stdout, REGISTER_FMT, name, value);
                found = 1;

                break;
            }
        }

        if (!found) {
            fprintf(stderr, "unknown register: %s\n", name);
        }
    }
}

void cli_set_regs(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);

    if (input->argc == 0) {
        return;
    }

    dbg_regs_t regs;

    const dbg_result_t result = dbg_set_regs(dbg, &regs); 
    const char* message = dbg_result_str(result);
    if (result != DBG_OK) {
        fprintf(stderr, ERROR_FMT, message); 
    }
}
