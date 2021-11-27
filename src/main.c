#include "../inc/ush.h"

void mx_set_shell_vars() {

    if (!getenv(PWD_STR)) {
        setenv(PWD_STR, getcwd(NULL, 0), 1);
    }

    int shlvl = mx_atoi(getenv("SHLVL"));
    char* shlvl_str = mx_itoa(shlvl + 1);
    setenv("SHLVL", shlvl_str, 1);
    mx_strdel(&shlvl_str);

    setenv("SHELL", "ush", 1); // probably a different shell path
    // setenv("PROMPT", "u$h", 1);

}

void mx_ush_init(t_cmd_utils** utils) {

    (*utils)->args = NULL;
    (*utils)->env_vars = NULL;
    (*utils)->exported_vars = NULL;
    
    // mx_set_shell_vars();
    mx_env_reset(utils);
    mx_export_reset(utils);
    
}

int main() {

    int status = 0;
    t_cmd_utils* utils = malloc(sizeof(*utils));
    mx_ush_init(&utils);

    while (1) {

        printf("%s", "u$h> ");
        char** lines = mx_read_line();
        for (int i = 0; lines[i] != NULL; ++i) {

            mx_parse_line(utils, lines[i]);
            status = mx_exec(utils);

        }
        mx_del_strarr(&lines);

    }

    mx_exit(utils);

}

// make prompt go on a newline, if there was none before
// echo "cd test ; pwd" | ./ush
// fix check_odd_quotes
