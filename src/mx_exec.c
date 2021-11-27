#include "../inc/ush.h"

int mx_builtin_exec(t_cmd_utils* utils) {
    
    for (int i = 0; builtin_cmds[i] != NULL; ++i) {

        if (mx_strcmp(utils->args[0], builtin_cmds[i]) == 0) {
            return (builtin_funcs[i])(utils);
        }

    }
    return 1;

}

int mx_exec(t_cmd_utils* utils) {

    pid_t pid;
    if (utils->args[0] == NULL || mx_builtin_exec(utils) == 0) {
        return 1;
    }

    pid = fork();
    int status = 0;

    if (pid == -1) {
        mx_print_cmd_err("fork", strerror(errno));
        mx_printerr("\n");
        exit(1);
    }
    if (pid == 0) {

        char** paths = mx_get_exec_paths(utils->args[0], NULL, true);
        char** env_var_array = mx_get_env_array(utils->exported_vars);
        char* utility = paths[0] ? paths[0] : utils->args[0];
        if (execve(utility, utils->args, env_var_array) == -1) {
            
            if (errno == ENOENT) {
                mx_printstr("ush: command not found: ");
                mx_printerr(utils->args[0]);
                mx_printerr("\n");
            } else {
                mx_print_cmd_err(utils->args[0], strerror(errno));
                mx_printerr("\n");
            }
            exit(1);

        }
        mx_del_strarr(&paths);
        mx_del_strarr(&env_var_array);
        return 0;

    }
    waitpid(pid, &status, 0); // handle options (3rd param) later
    return 1;

}
