#include "../inc/ush.h"

int mx_builtin_exec(t_cmd_utils* utils) {

    for (int i = 0; builtin_cmds[i] != NULL; ++i) {

        if (mx_strncmp(utils->cmd, builtin_cmds[i], 3) == 0) {
            return (builtin_funcs[i])(utils);
        }

    }
    return 1;

}

int mx_exec(t_cmd_utils* utils) {

    if (utils->cmd == NULL || mx_builtin_exec(utils) == 0) {
        return 1;
    }

    pid_t pid = fork();
    int status = 0;

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {

        utils->args[0] = mx_strdup("");
        if (execve("/bin/env", utils->args, utils->env_vars) == -1) {

            perror(utils->cmd);
            // cleanup for utils here
            exit(1);

        }
        return 0;

    }
    waitpid(pid, &status, 0); // handle options (3rd param) later
    return 1;

}
