#pragma once
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <unistd.h>

#include "util.h"

typedef enum { 
    OS_OK,

    OS_ERR_PTRACE,
    OS_ERR_FORK,
    OS_ERR_EXEC,
    OS_ERR_DETACH,
    OS_ERR_WAIT,

    OS_ERR_CONTINUE,
    OS_ERR_SINGLE_STEP,
    OS_ERR_READ_MEM,
    OS_ERR_WRITE_MEM,
    OS_ERR_GET_REGS,
    OS_ERR_SET_REGS,
} os_result_t;

NODISCARD os_result_t os_traceme(void);
NODISCARD os_result_t os_fork(pid_t* out_pid);
NODISCARD os_result_t os_exec(const char* path, char* const argv[]);
NODISCARD os_result_t os_detach(const pid_t pid);
NODISCARD os_result_t os_wait(const pid_t pid, int* out_status);

NODISCARD os_result_t os_continue(const pid_t pid, const int signal);
NODISCARD os_result_t os_single_step(const pid_t pid);
NODISCARD os_result_t os_read_mem(const pid_t pid, const unsigned long addr, unsigned long* out_data);
NODISCARD os_result_t os_write_mem(const pid_t pid, const unsigned long addr, const unsigned long data);
NODISCARD os_result_t os_get_regs(const pid_t pid, struct user_regs_struct* out_regs);
NODISCARD os_result_t os_set_regs(const pid_t pid, struct user_regs_struct* regs);

