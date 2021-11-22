#include "../inc/ush.h"

int mx_exit(t_cmd_utils* utils) {

    mx_clear_list(&utils->env_vars);
    
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
