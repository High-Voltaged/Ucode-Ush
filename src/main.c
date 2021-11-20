#include "../inc/ush.h"

void mx_ush_init(t_cmd_utils** utils) {

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
        mx_parse_line(utils, line);
        mx_strdel(&line);

        status = mx_exec(utils);

    }

    mx_exit(utils);

}

// make prompt go on a newline, if there was none before
