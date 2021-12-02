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

static void print_tilde_str(const char* curr_wd) {

    char* temp = mx_replace_substr(curr_wd, getenv(HOME_STR), "~");
    mx_printstr(temp);
    mx_printstr("\n");
    mx_strdel(&temp);

}

static char* replace_cd_arg(const char* sub, const char* to_replace) {

    char* pwd = getenv(PWD_STR);
    if (mx_strstr(pwd, sub) == NULL) {
        mx_printerr("cd: string not in pwd: ");
        mx_printerr(sub);
        mx_printerr("\n");
        return NULL;
    }

    return mx_replace_substr(pwd, sub, to_replace);

}

int mx_cd(t_cmd_utils* utils) {

    t_cd_flags* flags = malloc(sizeof(*flags));
    int arg_idx = 1;
    mx_cd_parse_flags(&flags, utils, &arg_idx);

    char* dir_str = NULL;
    bool is_replaceable = utils->args[arg_idx] && utils->args[arg_idx + 1];
    if (!flags->prev && is_replaceable) {

        dir_str = replace_cd_arg(utils->args[arg_idx], utils->args[arg_idx + 1]);
        if (!dir_str) return 0;
            
    } else if (flags->prev) {

        dir_str = getenv(OLDPWD_STR) ? mx_strdup(getenv(OLDPWD_STR)) : mx_strdup(getenv(HOME_STR));
    
    } else {

        dir_str = utils->args[arg_idx] ? mx_strdup(utils->args[arg_idx]) : mx_strdup(getenv(HOME_STR));

    }

    char* cwd = malloc(sizeof(char) * PATH_MAX);
    cwd = getcwd(cwd, PATH_MAX);
    char *path = mx_strdup(cwd);
    cwd = flags->P ? cwd : mx_normalize_path(path, getenv(PWD_STR));

    setenv(OLDPWD_STR, cwd, 1);
    free(cwd);
    
    if (chdir(dir_str) == -1) {
        char* err_str = mx_strdup(strerror(errno));
        err_str[0] = mx_tolower(err_str[0]);
        mx_print_cmd_err("cd", err_str);
        mx_printerr(": ");
        mx_printerr(dir_str);
        mx_printerr("\n");
        mx_strdel(&err_str);
        return 0;
    }

    char* curr_wd = malloc(sizeof(char) * PATH_MAX);
    curr_wd = flags->P ? getcwd(curr_wd, PATH_MAX) : mx_normalize_path(path, dir_str);
    
    if (/*flags->prev ||*/ is_replaceable) {
        print_tilde_str(curr_wd);
    }
    setenv(PWD_STR, curr_wd, 1);
    
    free(curr_wd);
    mx_strdel(&dir_str);
    mx_strdel(&path);
    free(flags);
    mx_env_reset(&utils);
    // mx_export_reset(&utils);
    return 0;

}
