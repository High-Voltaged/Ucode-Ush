#include "../inc/ush.h"

// Get arguments for the utility to be executed
char** mx_get_env_util_args(t_cmd_utils* utils, int util_arg_idx) {

    int arg_count = 1;
    for (int i = util_arg_idx; utils->args[i] != NULL; ++arg_count, ++i);

    char** util_args = malloc(sizeof(char*) * (arg_count + 1));
    util_args[0] = mx_strdup(utils->args[util_arg_idx - 1]);
    for (int i = 1, j = util_arg_idx; i < arg_count; ++i, ++j) {

        util_args[i] = mx_strdup(utils->args[j]);

    }
    util_args[arg_count] = NULL;
    return util_args;

}

// Execute the env's utility
int exec_env_utility(t_cmd_utils* utils, const char* env_util, int util_arg_idx, const char* custom_path) {

    // if no utility found, return
    if (utils->args[util_arg_idx - 1] == NULL)
        return 0;

    char** util_args = mx_get_env_util_args(utils, util_arg_idx);
    pid_t pid = fork();
    int status = 0;

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        
        char** paths = mx_get_exec_paths(env_util, custom_path, true);
        if (execve(paths[0] ? paths[0] : env_util, util_args, utils->env_vars) == -1) {
            // check for the flag -i

            perror(env_util);
            return 1;

        }
        mx_del_strarr(&paths);
        mx_print_env_vars(utils);
        return 0;

    }
    waitpid(pid, &status, 0);
    if (util_args)
        mx_del_strarr(&util_args);
    return 0;

}

// Get all possible (or just one of) paths to a binary file
char** mx_get_exec_paths(const char* to_find, const char* custom_path, bool single_search) {

    int path_count = 2;
    char** paths = malloc(sizeof(*paths) * path_count);
    paths[0] = paths[1] = NULL;

    DIR* curr_dir = NULL;
    struct dirent* dp = NULL; 

    const char* path_to_search_in = custom_path ? custom_path : getenv(PATH_STR);
    char** dirs = mx_strsplit(path_to_search_in, ':');
    int is_found = 0;
    for (int i = 0; !is_found && dirs[i] != NULL; ++i) {

        if ((curr_dir = opendir(dirs[i])) == NULL) 
            continue;

        while ((dp = readdir(curr_dir)) != NULL) {

            if (mx_strcmp(to_find, dp->d_name) == 0) {

                if (!single_search) {

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
    return paths;

}
