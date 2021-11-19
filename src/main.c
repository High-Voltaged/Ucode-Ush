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

// Don't count empty string as an argument (during parsing)
