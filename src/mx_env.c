#include "../inc/ush.h"

void mx_env_add_flag(t_env_flags** flags, char flag) {

    switch (flag) { 

    case 'i':
        (*flags)->i = 1; break;

    case 'P':
        (*flags)->P = 1; break;

    case 'u':
        (*flags)->u = 1; break;
    
    default: break;
    
    }

}

static void print_env_vars(t_cmd_utils* utils) {

    for (int i = 0; utils->env_vars[i] != NULL; ++i) {

        printf("%s\n", utils->env_vars[i]);

    }

}

static void set_env_vars(t_cmd_utils* utils, int* arg_idx) {

    for (int i = *arg_idx; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if (mx_strstr(arg, "=") != NULL) {

            char* var_name = mx_strndup(arg, mx_get_char_index(arg, '='));
            char* var_value = mx_strchr(arg, '=') + 1;

            setenv(var_name, var_value, 1);
            mx_strdel(&var_name);

        } else break;
        ++(*arg_idx);

    }

}

static void unset_env_var(t_cmd_utils** utils, int* arg_idx) {

    if (mx_strchr((*utils)->args[*arg_idx], '=') != NULL) {
        mx_printerr(ENV_UNSET_ERR);
        mx_printerr((*utils)->args[*arg_idx]);
        mx_printerr(ENV_INVARG_ERR);
        return;
    }

    unsetenv((*utils)->args[*arg_idx]);
    mx_env_reset(utils);
    ++(*arg_idx);
    // print_env_vars((*utils));

}

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

static int exec_env_utility(t_cmd_utils* utils, const char* env_util, int util_arg_idx, int flag_i_on) {

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
        
        char** paths = mx_get_exec_paths(env_util, true);
        if (execve(paths[0] ? paths[0] : env_util, util_args, utils->env_vars) == -1) {
            // check for the flag -i

            perror(env_util);
            return 1;

        }
        mx_del_strarr(&paths);
        print_env_vars(utils);
        return 0;

    }
    waitpid(pid, &status, 0);
    if (util_args)
        mx_del_strarr(&util_args);
    return 0;

}

int mx_env(t_cmd_utils* utils) {

    int curr_arg_idx = 1;
    t_env_flags* flags = malloc(sizeof(*flags));
    if (utils->args[1] != NULL) {

        mx_env_parse_flags(&flags, utils, &curr_arg_idx);

        if (flags->u) {
            unset_env_var(&utils, &curr_arg_idx);            
        }
        
        if (flags->i) {

            mx_clear_env_vars(&utils);
            set_env_vars(utils, &curr_arg_idx);
            exec_env_utility(utils, utils->args[curr_arg_idx], curr_arg_idx + 1, true);
            mx_env_reset(&utils);
        
        } else {
            set_env_vars(utils, &curr_arg_idx);
            exec_env_utility(utils, utils->args[curr_arg_idx], curr_arg_idx + 1, false);
            mx_env_reset(&utils);
        }

    } 
    if (!flags->i || utils->args[curr_arg_idx] == NULL) {

        print_env_vars(utils);
    
    }
    free(flags);

    return 0;

}
