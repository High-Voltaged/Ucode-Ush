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

int cd_prev(t_cmd_utils** utils, bool flag_p_on) {

    char* oldpwd = malloc(sizeof(char) * PATH_MAX);
    oldpwd = getcwd(oldpwd, PATH_MAX);
    char *path = mx_strdup(oldpwd);

    char* prev_wd = getenv(OLDPWD_STR);
    // prev_wd = flag_p_on ? getenv(OLDPWD_STR) : mx_normalize_path(path, getenv(OLDPWD_STR));
    // char* curr_wd = malloc(sizeof(char) * PATH_MAX);
    // curr_wd = flag_p_on ? mx_strdup(prev_wd) : mx_normalize_path(path, prev_wd);
    // printf("%s %d -- curr\n", prev_wd, flag_p_on);
    chdir(prev_wd);

    setenv(OLDPWD_STR, oldpwd, 1);
    setenv(PWD_STR, prev_wd, 1);
    
    // if (!flag_p_on)
        // mx_strdel(&prev_wd);
    mx_strdel(&oldpwd);
    mx_env_reset(utils);
    return 0;

}

int cd_home(t_cmd_utils* utils) {

    char* home = getenv(HOME_STR);
    char* cwd = malloc(sizeof(char) * PATH_MAX);

    getcwd(cwd, PATH_MAX);
    setenv(OLDPWD_STR, cwd, 1);

    chdir(home);

    setenv(PWD_STR, home, 1);

    mx_strdel(&cwd);
    mx_env_reset(&utils);
    return 0;

}

int mx_cd(t_cmd_utils* utils) {

    t_cd_flags* flags = malloc(sizeof(*flags));
    if (mx_cd_parse_flags(&flags, utils) != 0)
        return 0;

    char* dir_str = NULL;
    if (flags->P) {
        dir_str = utils->args[2] ? mx_strdup(utils->args[2]) : NULL;
    } else {
        dir_str = utils->args[1] ? mx_strdup(utils->args[1]) : NULL;
    }
    if (dir_str != NULL) {

        if (flags->prev) {

            mx_strdel(&dir_str);
            free(flags);
            return cd_prev(&utils, flags->P == 1);
            
        } else {

            char* cwd = malloc(sizeof(char) * PATH_MAX);
            cwd = getcwd(cwd, PATH_MAX);
            char *path = mx_strdup(cwd);
            cwd = flags->P ? cwd : mx_normalize_path(path, getenv(PWD_STR));

            setenv(OLDPWD_STR, cwd, 1);
            mx_strdel(&cwd);
            
            if (chdir(dir_str) == -1) {
                mx_print_cmd_err("cd", strerror(errno));
                return 0;
            }

            char* curr_wd = malloc(sizeof(char) * PATH_MAX);
            curr_wd = flags->P ? getcwd(curr_wd, PATH_MAX) : mx_normalize_path(path, dir_str);
            setenv(PWD_STR, curr_wd, 1);
            
            mx_strdel(&curr_wd);
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
