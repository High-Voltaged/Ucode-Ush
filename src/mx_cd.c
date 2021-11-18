#include "../inc/ush.h"

static void add_flag(t_cd_flags** flags, char flag) {

    switch (flag) { 

    case 's':
        (*flags)->s = 1; break;

    case 'P':
        (*flags)->P = 1; break;

    case '-':
        (*flags)->prev = 1; break;
    
    default: break;
    
    }

}

static void flag_init(t_cd_flags* flags, int count) {

    for (int i = 0; i < count; ++i) {
        ((&flags->s)[i]) = 0;
    }

}

void mx_parse_flags(t_cd_flags** flags, t_cmd_utils* utils) {

    char* const_flags = "sP";
    flag_init(*flags, 3);

    if (utils->args == NULL) return;

    for (int i = 0; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if (arg[0] == '-' && mx_strlen(arg) == 1) {
            add_flag(flags, arg[0]);
            continue;
        } 
        if ((arg[0] == '-') && mx_isalpha(arg[1])) {

            for (int j = 1; arg[j] != '\0'; j++) {

                if (mx_is_flag_found(const_flags, arg[j])) {
                    add_flag(flags, arg[j]);
                } else {
                    mx_printerr("invalid option -- ");
                    mx_printerr(arg);
                    mx_printerr("\n");
                    exit(1);
                }
            
            }
        }
    }
}

int cd_prev(t_cmd_utils** utils) {

    char* cwd = malloc(sizeof(char) * PATH_MAX);
    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd");
        return 1;
    }
    char* prev_wd = getenv("OLDPWD");
    chdir(prev_wd);

    setenv("OLDPWD", cwd, 1);

    setenv("PWD", prev_wd, 1);
    // mx_strdel(&prev_wd);
    mx_strdel(&cwd);
    mx_env_reset(utils);
    return 1;

}

int cd_home(t_cmd_utils* utils) {

    char* home = getenv("HOME");
    char* cwd = malloc(sizeof(char) * PATH_MAX);

    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd");
        return 1;
    }
    setenv("OLDPWD", cwd, 1);

    chdir(home);

    setenv("PWD", home, 1);
    // mx_strdel(&home);
    mx_strdel(&cwd);
    mx_env_reset(&utils);
    return 1;

}

int mx_cd(t_cmd_utils* utils) {

    t_cd_flags* flags = malloc(sizeof(*flags));
    mx_parse_flags(&flags, utils);

    if (utils->args != NULL) {

        if (flags->prev) {

            return cd_prev(&utils);
            
        } else {

            char* cwd = malloc(sizeof(char) * PATH_MAX);
            cwd = getcwd(cwd, PATH_MAX);
            setenv("OLDPWD", cwd, 1);
            if (chdir(utils->args[0]) == -1) {
                perror("chdir");
                exit(1);
            }
            mx_strdel(&cwd);
            cwd = malloc(sizeof(char) * PATH_MAX);
            getcwd(cwd, PATH_MAX);
            setenv("PWD", cwd, 1);
            mx_strdel(&cwd);
            mx_env_reset(&utils);

            return 1;

        }

    } else {

        return cd_home(utils);

    }

}
