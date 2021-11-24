#include "../inc/ush.h"

void mx_cd_add_flag(t_cd_flags** flags, char flag) {

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

int cd_prev(t_cmd_utils** utils) {

    char* cwd = malloc(sizeof(char) * PATH_MAX);
    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd");
        return 1;
    }
    char* prev_wd = getenv(OLDPWD_STR);
    chdir(prev_wd);

    setenv(OLDPWD_STR, cwd, 1);

    setenv(PWD_STR, prev_wd, 1);
    // mx_strdel(&prev_wd);
    mx_strdel(&cwd);
    mx_env_reset(utils);
    return 0;

}

int cd_home(t_cmd_utils* utils) {

    char* home = getenv(HOME_STR);
    char* cwd = malloc(sizeof(char) * PATH_MAX);

    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("getcwd");
        return 1;
    }
    setenv(OLDPWD_STR, cwd, 1);

    chdir(home);

    setenv(PWD_STR, home, 1);
    // mx_strdel(&home);
    mx_strdel(&cwd);
    mx_env_reset(&utils);
    return 0;

}

int mx_cd(t_cmd_utils* utils) {

    t_cd_flags* flags = malloc(sizeof(*flags));
    mx_cd_parse_flags(&flags, utils);

    char* dir_str = NULL;
    if (flags->P)  {
        dir_str = utils->args[2] ? mx_strdup(utils->args[2]) : NULL;
    } else {
        dir_str = utils->args[1] ? mx_strdup(utils->args[1]) : NULL;
    }
    if (dir_str != NULL) {

        if (flags->prev) {

            mx_strdel(&dir_str);
            free(flags);
            return cd_prev(&utils);
            
        } else {

            char* cwd = malloc(sizeof(char) * PATH_MAX);
            cwd = getcwd(cwd, PATH_MAX);
            char *path = mx_strdup(cwd);
            setenv(OLDPWD_STR, cwd, 1);
            
            if (chdir(dir_str) == -1) {
                perror("chdir");
                return 0;
            }
            mx_strdel(&cwd);

            cwd = malloc(sizeof(char) * PATH_MAX);
            cwd = flags->P ? getcwd(cwd, PATH_MAX) : mx_normalize_path(path, dir_str);
            setenv(PWD_STR, cwd, 1);
            
            mx_strdel(&cwd);
            mx_strdel(&dir_str);
            mx_strdel(&path);
            free(flags);
            mx_env_reset(&utils);
            return 0;

        }

    } else {

        free(flags);
        return cd_home(utils);

    }

}
