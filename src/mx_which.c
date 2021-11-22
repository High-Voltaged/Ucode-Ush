#include "../inc/ush.h"

void mx_wch_add_flag(t_wch_flags** flags, char flag) {

    switch (flag) { 

    case 's':
        (*flags)->s = 1; break;

    case 'a':
        (*flags)->a = 1; break;
    
    default: break;
    
    }

}

char* get_dir_path(char* dir, const char* file_name) {

    char* path = mx_strjoin(dir, "/");
    mx_strcat(path, file_name);
    return path;    

}

static bool is_builtin_cmd(const char* cmd) {

    for (int i = 0; builtin_cmds[i] != NULL; ++i) {

        if (mx_strcmp(cmd, builtin_cmds[i]) == 0) {
            return true;
        }

    }
    return false;

}

int mx_which(t_cmd_utils* utils) {

    const char* to_find = utils->args[2] ? mx_strdup(utils->args[2]) : mx_strdup(utils->args[1]);

    if (to_find == NULL)
        return 0;

    if (is_builtin_cmd(to_find)) {
        mx_printerr(to_find);
        mx_printerr(BUILTIN_CMD_ERR);
        return 0;
    }

    t_wch_flags* flags = malloc(sizeof(*flags));
    mx_wch_parse_flags(&flags, utils);

    char** paths = mx_get_exec_paths(to_find, NULL, !flags->a);

    if (paths[0] == NULL) {
        mx_printerr(to_find);
        mx_printerr(" not found\n");
    } else {
        mx_print_strarr(paths, "\n");
    }

    mx_del_strarr(&paths);
    free(flags);

    return 0;

}
