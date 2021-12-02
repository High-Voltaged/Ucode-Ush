#include "../inc/ush.h"

int mx_builtin_exec(t_cmd_utils* utils) {
    
    for (int i = 0; builtin_cmds[i] != NULL; ++i) {

        if (mx_strcmp(utils->args[0], builtin_cmds[i]) == 0) {
            return (builtin_funcs[i])(utils);
        }

    }
    return 1;

}

void mx_exec(t_cmd_utils* utils) {

    pid_t pid;
    if (utils->args[0] == NULL || mx_builtin_exec(utils) == 0) {
        return;
    }

    mx_process_push_back(&utils->processes, utils);
    // mx_print_process_list(utils->processes);

    pid = fork();
    int status = 0;

    if (pid == -1) {
        mx_print_cmd_err("fork", strerror(errno));
        mx_printerr("\n");
        exit(1);
    }
    
    t_process* p = mx_top_process(utils->processes);
    if (pid == 0) {

        if (utils->is_interactive) {
            
            pid_t cpid = getpid();
            setpgid(cpid, cpid);
            tcsetpgrp (0, cpid);
            mx_signals_init(SIG_DFL);
        
        }
        char** env_var_array = mx_get_env_array(utils->exported_vars);
        if (execve(p->path, utils->args, env_var_array) == -1) {
            
            if (errno == ENOENT) {
                mx_printstr("ush: command not found: ");
                mx_printerr(utils->args[0]);
                mx_printerr("\n");
            } else {
                mx_print_cmd_err(utils->args[0], strerror(errno));
                mx_printerr("\n");
            }
            p->status = 1;
            exit(1);

        }
        mx_del_strarr(&env_var_array);
        // exit(0);

    } else {

        p->pid = pid;
        if (utils->is_interactive) {
            setpgid(pid, pid);
        }
    
    }

    if (!utils->is_interactive)
        mx_wait_for_job(utils);
    else
        mx_foreground_job(utils, p, 0);

//  error processes not getting removed

    if (WIFEXITED(p->status) && WEXITSTATUS(p->status) != 0) {
        printf("popping process, exit status -- %d\n", p->status);
        mx_process_pop_back(&utils->processes);
    }

    // exit(status);

}
