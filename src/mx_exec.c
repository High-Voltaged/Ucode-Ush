#include "../inc/ush.h"

int mx_builtin_exec(t_cmd_utils* utils) {

    for (int i = 0; builtin_cmds[i] != NULL; ++i) {

        int cmd_len = mx_get_char_index(utils->cmd, '\n') - 1;
        if (mx_strncmp(utils->cmd, builtin_cmds[i], cmd_len) == 0) {
            return (builtin_funcs[i])(utils);
        }

    }
    return 1;

}

int mx_exec(t_cmd_utils* utils) {

    pid_t pid;
    if (utils->cmd == NULL || mx_builtin_exec(utils) == 0) {
        return 1;
    }

    pid = fork();
    int status = 0;

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {

        if (execvp(utils->cmd, utils->args) == -1) {

            perror(utils->cmd);
            exit(1);

        }
        return 0;

    }
    waitpid(pid, &status, 0); // handle options (3rd param) later
    return 1;

}
