#include "../inc/ush.h"

void mx_env_reset(t_cmd_utils** utils) {

    int i = 0;

    for (; environ[i] != NULL; ++i) {

        if ((*utils)->env_vars[i])
            mx_strdel(&(*utils)->env_vars[i]);
            
        (*utils)->env_vars[i] = mx_strdup(environ[i]);

    }
    (*utils)->env_vars[i] = NULL;
    
}

void mx_clear_env_vars(t_cmd_utils** utils) {

    for (int i = 0; (*utils)->env_vars[i]; ++i) {
            
        if ((*utils)->env_vars[i])
            mx_strdel(&(*utils)->env_vars[i]);
    
    }

}
