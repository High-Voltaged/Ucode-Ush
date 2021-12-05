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

void mx_set_env_vars(t_cmd_utils* utils, char** args, int* arg_idx) {

    for (int i = *arg_idx; args[i] != NULL; ++i) {

        char* arg = args[i];
        if (mx_strstr(arg, "=") != NULL) {
            char* var_name = mx_get_var_name(arg);
            t_env_var* env_var = mx_find_env_var(utils->env_vars, var_name, NULL);
            
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

int mx_remove_env_var(t_cmd_utils** utils, char* name) {

    if (mx_strchr(name, '=') != NULL) {
        mx_printerr("env: cannot unset '");
        mx_printerr(name);
        mx_printerr("': Invalid argument\n");
        return 1;
    }

    int env_index = 0;
    int export_index = 0;
    mx_find_env_var((*utils)->env_vars, name, &env_index);
    mx_find_env_var((*utils)->exported_vars, name, &export_index);
    mx_env_pop_index(&(*utils)->env_vars, env_index);
    mx_env_pop_index(&(*utils)->exported_vars, export_index);
    return 0;

}

int mx_env(t_cmd_utils* utils, char** args) {

    int curr_arg_idx = 1;
    t_env_flags* flags = malloc(sizeof(*flags));
    if (mx_env_parse_flags(&flags, args, &curr_arg_idx) != 0)
        return 0;

    if (args[1] != NULL) {

        if (args[curr_arg_idx]) {

            mx_exec_env_utility(utils, args, curr_arg_idx, flags);

        }

    } else {

        mx_print_env_list(utils->env_vars, true);
    
    }
    free(flags);

    return 0;

}
