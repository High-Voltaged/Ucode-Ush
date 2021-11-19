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

static char* get_dir_path(char* dir, const char* file_name) {

    char* path = mx_strjoin(dir, "/");
    mx_strcat(path, file_name);
    return path;    

}

int mx_which(t_cmd_utils* utils, char* to_find) {

    if (to_find == NULL || mx_strcmp(to_find, "") == 0)
        return 0;

    t_wch_flags* flags = malloc(sizeof(*flags));
    // mx_wch_parse_flags(&flags, utils);
    flags->a = 1;
    flags->s = 0;

    int path_count = 2;
    char** paths = malloc(sizeof(*paths) * path_count);
    paths[0] = paths[1] = NULL;

    DIR* curr_dir = NULL;
    struct dirent* dp = NULL; 
    char** dirs = mx_strsplit(getenv(PATH_STR), ':');
    int is_found = 0;
    for (int i = 0; !is_found && dirs[i] != NULL; ++i) {

        if ((curr_dir = opendir(dirs[i])) == NULL) 
            continue;

        while ((dp = readdir(curr_dir)) != NULL) {

            if (mx_strcmp(to_find, dp->d_name) == 0) {

                if (flags->a) {

                    if (paths[path_count - 2] != NULL) {
                        paths = mx_realloc(paths, sizeof(*paths) * (++path_count));
                    }
                    paths[path_count - 2] = get_dir_path(dirs[i], to_find);
                    continue;
                
                }
                paths[0] = get_dir_path(dirs[i], to_find);
                is_found = 1;
                break;

            }

        }
        closedir(curr_dir);

    }
    mx_del_strarr(&dirs);
    mx_print_strarr(paths, "\n");
    mx_del_strarr(&paths);
    free(flags);

    return 0;

}
