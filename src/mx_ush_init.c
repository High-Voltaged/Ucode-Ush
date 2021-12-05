#include "../inc/ush.h"

void mx_signals_init(sig_t handler) {

    signal (SIGINT, handler);
    signal (SIGQUIT, handler);
    signal (SIGTSTP, handler);
    signal (SIGTTIN, handler);
    signal (SIGTTOU, handler);

}

static void set_shell_vars() {

    if (!getenv(PWD_STR)) {
        setenv(PWD_STR, getcwd(NULL, 0), 1);
    }
    if (!getenv(OLDPWD_STR)) {
        setenv(OLDPWD_STR, getcwd(NULL, 0), 1);
    }

    int shlvl = mx_atoi(getenv("SHLVL"));
    char* shlvl_str = mx_itoa(shlvl + 1);
    setenv("SHLVL", shlvl_str, 1);
    mx_strdel(&shlvl_str);

    // char* shell_path = realpath("ush", NULL);
    // setenv("SHELL", shell_path, 1);
    // mx_strdel(&shell_path);

}

static void shell_init(t_cmd_utils** utils) {

    int sh_tty = STDIN_FILENO;
    int shell_pgid;
    bool shell_is_interactive = isatty(sh_tty);
    if (shell_is_interactive) {
        
        while (tcgetpgrp (sh_tty) != (shell_pgid = getpgrp()))
            kill (- shell_pgid, SIGTTIN);

        mx_signals_init(SIG_IGN);

        shell_pgid = getpid();
        if (setpgid (shell_pgid, shell_pgid) < 0) {
            exit(EXIT_FAILURE);
        }

        (*utils)->shell_pgid = shell_pgid;
        tcsetpgrp (sh_tty, shell_pgid);
        tcgetattr (sh_tty, &(*utils)->shell_modes);
    
    }
    (*utils)->processes = (*utils)->stopped_jobs = NULL;
    (*utils)->is_interactive = shell_is_interactive;

}

void mx_ush_init(t_cmd_utils** utils) {

    shell_init(utils);

    (*utils)->args = NULL;
    (*utils)->env_vars = NULL;
    (*utils)->exported_vars = NULL;
    (*utils)->processes = NULL;
    (*utils)->stopped_jobs = NULL;
    
    set_shell_vars();
    mx_env_reset(utils);
    mx_export_reset(utils);
    
}
