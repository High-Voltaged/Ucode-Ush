#include "../inc/ush.h"

void mx_env_reset(t_cmd_utils** utils) {

    int i = 0;
    for (; environ[i] != NULL; ++i) {

        (*utils)->env_vars[i] = mx_strdup(environ[i]);

    }
    (*utils)->env_vars[i] = NULL;
    
}

void mx_ush_init(t_cmd_utils** utils) {

    (*utils)->cmd = NULL;
    (*utils)->args = NULL;
    
    mx_env_reset(utils);
    
}

int main() {

    int status = 0;
    t_cmd_utils* utils = malloc(sizeof(*utils));
    mx_ush_init(&utils);


    while (1) {

        printf("u$h> ");
        char* line = mx_read_line();
        parse_line(utils, line);
        utils->cmd = mx_strdup(line);
        mx_strdel(&line);

        status = mx_exec(utils);

        mx_strdel(&utils->cmd);

    }

    mx_exit(utils);

}
