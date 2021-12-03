#include "../inc/ush.h"

int mx_exit(t_cmd_utils* utils) {

    static int warning = 0;
    if (utils->stopped_jobs != NULL && warning != 1) {
        mx_printerr("ush: you have suspended jobs.\n");
        warning = 1;
        return 0;
    }

    mx_env_clear_list(&utils->env_vars);
    mx_env_clear_list(&utils->exported_vars);
    mx_clear_process_list(&utils->processes);
    
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
