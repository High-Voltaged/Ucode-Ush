#include "../inc/ush.h"

int mx_builtin_exec(t_cmd_utils* utils, char** args) {
    
    for (int i = 0; builtin_cmds[i] != NULL; ++i) {

        if (mx_strcmp(args[0], builtin_cmds[i]) == 0) {
            return (builtin_funcs[i])(utils, args);
        }

    }
    return 1;

}

void mx_exec(t_cmd_utils* utils) {

    pid_t pid;
    if (utils->args[0] == NULL || mx_builtin_exec(utils, utils->args) == 0) {
        return;
    }

    mx_process_push_back(&utils->processes, utils, NULL);
    // mx_print_process_list(utils->processes);

    pid = fork();

    t_process* chld_process = mx_top_process(utils->processes, NULL);
    if (pid == -1) {
        mx_print_cmd_err("fork", strerror(errno));
        mx_printerr("\n");
        mx_process_exit(utils, utils->args, EXIT_FAILURE);
    }
    
    if (pid == 0) {

        if (utils->is_interactive) {
            
            pid_t cpid = getpid();
            setpgid(cpid, cpid);
            tcsetpgrp (0, cpid);
            mx_signals_init(SIG_DFL);
        
        }
        char** env_var_array = mx_get_env_array(utils->exported_vars);
        if (execve(chld_process->path, utils->args, env_var_array) == -1) {
            
            if (errno == ENOENT) {
                mx_printstr("ush: command not found: ");
                mx_printerr(utils->args[0]);
                mx_printerr("\n");
                mx_process_exit(utils, utils->args, MX_EXIT_ENOENT);
            } else {
                mx_print_cmd_err(utils->args[0], strerror(errno));
                mx_printerr("\n");
                mx_process_exit(utils, utils->args, EXIT_FAILURE);
            }

        }
        mx_del_strarr(&env_var_array);

    } else {

        chld_process->pid = pid;
        if (utils->is_interactive) {
            setpgid(pid, pid);
        }
    
    }

    if (!utils->is_interactive)
        mx_wait_for_job(utils, chld_process);
    else
        mx_foreground_job(utils, chld_process, 0);

    if (WIFEXITED(chld_process->status))
        printf("exit status of %d is %d\n", chld_process->pid, WEXITSTATUS(chld_process->status));

}
