#include "../inc/ush.h"

void mx_env_add_flag(t_env_flags** flags, char flag) {

    switch (flag) { 

    case 'i':
        (*flags)->i = 1; 
        (*flags)->P = 0; 
        break;

    case 'P':
        (*flags)->P = 1; break;

    case 'u':
        (*flags)->u = 1; break;
    
    default: break;
    
    }

}

void mx_set_env_vars(t_cmd_utils* utils, int* arg_idx) {

    for (int i = *arg_idx; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        char* var_name = mx_get_var_name(arg);
        t_env_var* env_var = mx_find_env_var(utils->env_vars, var_name, NULL);
        if (mx_strstr(arg, "=") != NULL) {

            if (env_var == NULL) {

                mx_env_push_back(&utils->env_vars, arg);

            } else {

                char* var_value = mx_get_var_value(arg);
                mx_overwrite_env_var(&env_var, var_value);
                mx_strdel(&var_name);
                mx_strdel(&var_value);

            }

        } else break;
        ++(*arg_idx);

    }

}

void mx_unset_env_var(t_cmd_utils** utils, int* arg_idx) {

    char* var_name = (*utils)->args[*arg_idx];
    if (mx_strchr(var_name, '=') != NULL) {
        mx_printerr(ENV_UNSET_ERR);
        mx_printerr(var_name);
        mx_printerr(ENV_INVARG_ERR);
        ++(*arg_idx);
        return;
    }

    int index = 0;
    mx_find_env_var((*utils)->env_vars, var_name, &index);
    mx_env_pop_index(&(*utils)->env_vars, index);
    ++(*arg_idx);

}

int mx_env(t_cmd_utils* utils) {

    int curr_arg_idx = 1;
    t_env_flags* flags = malloc(sizeof(*flags));
    if (utils->args[1] != NULL) {

        mx_env_parse_flags(&flags, utils, &curr_arg_idx);

        if (utils->args[curr_arg_idx]) {

            if (exec_env_utility(utils, curr_arg_idx, flags) != 0)
                mx_print_env_list(utils->env_vars, true);

        }

    } else {

        mx_print_env_list(utils->env_vars, true);
    
    }
    free(flags);

    return 0;

}
