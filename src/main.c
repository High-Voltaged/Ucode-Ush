#include "../inc/ush.h"

void mx_set_shell_vars() {

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
    
    mx_set_shell_vars();
    mx_env_reset(utils);
    mx_export_reset(utils);
    
}

int main() {

    int status = 0;
    t_cmd_utils* utils = malloc(sizeof(*utils));
    mx_ush_init(&utils);

    while (1) {

        printf("%s", PROMPT_STR);
        char* line = mx_read_line();
        mx_parse_line(utils, line);
        mx_strdel(&line);

        status = mx_exec(utils);

    }

    mx_exit(utils);

}

// make prompt go on a newline, if there was none before
// check `which -a` for both shell builtin & 'not found' error -- just output the error
// check `cd -P -` for symlinks & `cd - -P` for error -- output physical dir regardless
// add output for the result of `cd -`
// add single quotes for when exported var values contain neither alpha nor digits
// read_line handling '' ""
