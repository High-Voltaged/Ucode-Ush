#include "../inc/ush.h"

void mx_cleanup(t_cmd_utils* utils) {

    mx_env_clear_list(&utils->env_vars);
    mx_env_clear_list(&utils->exported_vars);
    mx_clear_process_list(&utils->processes);
    
    if (utils->args) {
        for (int i = 0; utils->args[i]; ++i) {
            mx_strdel(&utils->args[i]);
        }
        free(utils->args);
    }

} 

int mx_exit(t_cmd_utils* utils) {

    static int warning = 0;
    if (utils->stopped_jobs != NULL && warning != 1) {
        mx_printerr("ush: you have suspended jobs.\n");
        warning = 1;
        return 0;
    }

    t_process* p = mx_top_process(utils->processes, NULL);
    int exit_code = p ? p->status : EXIT_SUCCESS;

    mx_cleanup(utils);

    printf("Exiting...\n");
    // fflush(stdout);
    exit(exit_code);

}
