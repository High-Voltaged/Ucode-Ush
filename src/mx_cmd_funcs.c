#include "../inc/ush.h"

int mx_exit(t_cmd_utils* utils) {

    for (int i = 0; utils->env_vars[i]; ++i) {
        mx_strdel(&utils->env_vars[i]);
    }
    // free(utils->env_vars);
    if (utils->args) {
        for (int i = 0; utils->args[i]; ++i) {
            mx_strdel(&utils->args[i]);
        }
        free(utils->args);
    }

    printf("Exiting...\n");
    // fflush(stdout);
    exit(0);

}

int mx_env(t_cmd_utils* utils) {

    for (int i = 0; utils->env_vars[i] != NULL; ++i) {

        printf("%s\n", utils->env_vars[i]);

    }
    return 0;

}
