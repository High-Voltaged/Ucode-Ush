#include "../inc/ush.h"

void mx_ush_init(t_cmd_utils** utils) {

    (*utils)->cmd = NULL;
    (*utils)->args = NULL;
    
    int i = 0;
    for (; environ[i] != NULL; ++i) {

        (*utils)->env_vars[i] = mx_strdup(environ[i]);

    }
    (*utils)->env_vars[i] = NULL;
    
}

int main() {

    int status = 0;
    t_cmd_utils* utils = malloc(sizeof(*utils));
    mx_ush_init(&utils);


    while (1) {

        char* line = mx_read_line();
        utils->cmd = mx_strdup(line);
        mx_strdel(&line);

        status = mx_exec(utils);

        mx_strdel(&utils->cmd);

    }

    return status; 

}
