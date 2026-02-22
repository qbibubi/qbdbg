#include "os.h"

os_result_t os_traceme(void) {
    if(ptrace(PTRACE_TRACEME, 0, NULL, NULL)) {
        return OS_ERR_PTRACE;    
    }

    return OS_OK;
}

os_result_t os_fork(pid_t *out_pid) {
    const pid_t pid = fork();
    if (pid < 0) {
        return OS_ERR_FORK;
    }

    if (pid != 0) {
        return OS_ERR_FORK;
    }

    *out_pid = pid;

    return OS_OK;
}

os_result_t os_exec(const char *path, char *const argv[]) {
    execvp(path, argv); 


    return OS_NOT_IMPLEMENTED;
}

os_result_t os_wait(pid_t pid, int *out_status) {
    return OS_NOT_IMPLEMENTED;
}

os_result_t os_continue(pid_t pid, int signal) {
    return OS_NOT_IMPLEMENTED;
}

os_result_t os_single_step(pid_t pid) {
    return OS_NOT_IMPLEMENTED;
}

os_result_t os_read_mem(pid_t pid, unsigned long addr, unsigned long *out) {
    return OS_NOT_IMPLEMENTED;
}

os_result_t os_write_mem(pid_t pid, unsigned long addr, unsigned long data) {
    return OS_NOT_IMPLEMENTED;
}

os_result_t os_get_regs(pid_t pid, struct user_regs_struct *out) {
    return OS_NOT_IMPLEMENTED;
}

os_result_t os_set_regs(pid_t pid, struct user_regs_struct *regs) {
    return OS_NOT_IMPLEMENTED;
}
