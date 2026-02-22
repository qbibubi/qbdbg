#pragma once
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>

typedef enum { 
    OS_OK,
    OS_ERR_FORK,
    OS_ERR_PTRACE,
    OS_ERR_WAIT,
    OS_ERR_EXEC
} os_result_t;

os_result_t os_traceme(void);
os_result_t os_fork(pid_t *out_pid);
os_result_t os_exec(const char *path, char *const argv[]);
os_result_t os_wait(pid_t pid, int *out_status);
os_result_t os_continue(pid_t pid, int signal);
os_result_t os_single_step(pid_t pid);
os_result_t os_read_mem(pid_t pid, unsigned long addr, unsigned long *out);
os_result_t os_write_mem(pid_t pid, unsigned long addr, unsigned long data);
os_result_t os_get_regs(pid_t pid, struct user_regs_struct *out);
os_result_t os_set_regs(pid_t pid, struct user_regs_struct *regs);

