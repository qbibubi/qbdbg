#include "os.h"

os_result_t os_traceme(void) {
    const long int ptrace_result = ptrace(PTRACE_TRACEME, 0, NULL, NULL);

    // @TODO: I wonder whether compilers here will create branching and to avoid it do we use a
    // shorthand if to skip it?
    if (ptrace_result < 0) {
        return OS_ERR_PTRACE;    
    }

    return OS_OK;
}

os_result_t os_fork(pid_t *out_pid) {
    const pid_t pid = fork();
    if (pid < 0) {
        return OS_ERR_FORK;
    }

    *out_pid = pid;

    return OS_OK;
}

os_result_t os_exec(const char *path, char *const argv[]) {
    execvp(path, argv); 
    return OS_ERR_EXEC;
}

os_result_t os_detach(const pid_t pid) {
    kill(pid, SIGKILL);
    return OS_OK;
}

os_result_t os_wait(pid_t pid, int *out_status) {
    return OS_ERR_WAIT;
}

os_result_t os_continue(pid_t pid, int signal) {
    return OS_ERR_CONTINUE;
}

os_result_t os_single_step(pid_t pid) {
    return OS_ERR_SINGLE_STEP;
}

os_result_t os_read_mem(pid_t pid, unsigned long addr, unsigned long *out) {
    return OS_ERR_READ_MEM;
}

os_result_t os_write_mem(pid_t pid, unsigned long addr, unsigned long data) {
    return OS_ERR_WRITE_MEM;
}

os_result_t os_get_regs(pid_t pid, struct user_regs_struct *out) {
    return OS_ERR_GET_REGS;
}

os_result_t os_set_regs(pid_t pid, struct user_regs_struct *regs) {
    return OS_ERR_SET_REGS;
}
