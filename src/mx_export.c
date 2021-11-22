#include "../inc/ush.h"

int mx_export(t_cmd_utils* utils) {

    return 0;

}

void mx_export_reset(t_cmd_utils** utils) {

    int i = 0;

    mx_clear_list(&(*utils)->exported_vars);

    for (; environ && environ[i] != NULL; ++i) {
            
        mx_push_back(&(*utils)->exported_vars, environ[i]);

    }
    

}
