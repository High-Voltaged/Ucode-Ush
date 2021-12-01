#include "../inc/ush.h"

void mx_set_shell_vars() {

    if (!getenv(PWD_STR)) {
        setenv(PWD_STR, getcwd(NULL, 0), 1);
    }

    int shlvl = mx_atoi(getenv("SHLVL"));
    char* shlvl_str = mx_itoa(shlvl + 1);
    setenv("SHLVL", shlvl_str, 1);
    mx_strdel(&shlvl_str);

    setenv("SHELL", realpath("ush", NULL), 1);
    // setenv("PROMPT", "u$h", 1);

}

void mx_ush_init(t_cmd_utils** utils) {

    int shell_terminal = STDIN_FILENO;
    int shell_pgid;
    bool shell_is_interactive = isatty(shell_terminal);

    if (shell_is_interactive) {
        while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
            kill (- shell_pgid, SIGTTIN);

        //   signal (SIGINT, SIG_IGN);
        //   signal (SIGQUIT, SIG_IGN);
        //   signal (SIGTSTP, SIG_IGN);
        //   signal (SIGTTIN, SIG_IGN);
        //   signal (SIGTTOU, SIG_IGN);
        //   signal (SIGCHLD, SIG_IGN);

        shell_pgid = getpid();
        if (setpgid (shell_pgid, shell_pgid) < 0) {
            perror ("Couldn't put the shell in its own process group");
            exit (1);
        }

        (*utils)->shell_pgid = shell_pgid;
        tcsetpgrp (shell_terminal, shell_pgid);
        tcgetattr (shell_terminal, &(*utils)->shell_modes);
    
    }

    (*utils)->args = NULL;
    (*utils)->env_vars = NULL;
    (*utils)->exported_vars = NULL;
    (*utils)->processes = NULL;
    
    mx_set_shell_vars();
    mx_env_reset(utils);
    mx_export_reset(utils);
    
}

int main() {

    t_cmd_utils* utils = malloc(sizeof(*utils));
    mx_ush_init(&utils);

    while (1) {

        printf("%s", "u$h> ");
        char** lines = mx_read_line();
        for (int i = 0; lines[i] != NULL; ++i) {

            if (mx_parse_line(utils, lines[i]) != 0) 
                continue;

            mx_exec(utils);

        }
        mx_del_strarr(&lines);

    }

    mx_exit(utils);

}

// echo "cd test ; pwd" | ./ush
// fix check_odd_quotes
