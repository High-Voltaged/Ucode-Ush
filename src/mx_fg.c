#include "../inc/ush.h"

int mx_fg(t_cmd_utils* utils) {

    char* fg_arg = utils->args[1] ? mx_strdup(utils->args[1]) : mx_strdup("");

    if (fg_arg[0] == '%') {
        ++fg_arg;
    }
    t_process* proc_to_fg = NULL;
    if (mx_strlen(fg_arg) == 1 && mx_isdigit(fg_arg[0])) {

        printf("processing for nodeid\n");
        proc_to_fg = mx_get_process_by_nodeid(utils->stopped_jobs, mx_atoi(fg_arg));

    } else if (mx_strlen(fg_arg) > 0) {

        printf("processing for name\n");
        proc_to_fg = mx_get_process_by_name(utils->stopped_jobs, fg_arg);

    } else {

        printf("processing top\n");
        proc_to_fg = mx_top_process(utils->stopped_jobs);
        printf("%s -- top process name\n", proc_to_fg->path);

    }
    // mx_strdel(&fg_arg);

    mx_foreground_job(utils, proc_to_fg, 1);
    return 0;

}
