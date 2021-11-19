#include "../inc/ush.h"

int mx_builtin_exec(t_cmd_utils* utils) {

    if (mx_strcmp(utils->args[0], WHICH_STR) == 0) {
        mx_which(utils, utils->args[2] ? utils->args[2] : utils->args[1]);
        return 0;
    }

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
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        
        if (execvp(utils->args[0], utils->args) == -1) {
            
            perror(utils->args[0]);
            return 1;
            // exit(1);

        }
        return 0;

    }
    waitpid(pid, &status, 0); // handle options (3rd param) later
    return 1;

}
