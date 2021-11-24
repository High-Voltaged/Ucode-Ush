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
        if (mx_strstr(arg, "=") != NULL) {

            char* var_name = mx_strndup(arg, mx_get_char_index(arg, '='));
            char* var_value = mx_strchr(arg, '=') + 1;

            setenv(var_name, var_value, 1);
            mx_push_back(&utils->env_vars, arg);
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
        ++(*arg_idx);
        return;
    }

    unsetenv((*utils)->args[*arg_idx]);
    mx_pop_back(&(*utils)->env_vars);
    ++(*arg_idx);

}

int mx_env(t_cmd_utils* utils) {

    int curr_arg_idx = 1;
    t_env_flags* flags = malloc(sizeof(*flags));
    if (utils->args[1] != NULL) {

        mx_env_parse_flags(&flags, utils, &curr_arg_idx);

        if (flags->u) {
            unset_env_var(&utils, &curr_arg_idx);            
        }
        
        char* custom_path = flags->P ? mx_strdup(utils->args[curr_arg_idx++]) : NULL;
        if (utils->args[curr_arg_idx]) {

            if (exec_env_utility(utils, curr_arg_idx, custom_path, flags->i) != 0)
                mx_print_list(utils->env_vars);

        }
        mx_strdel(&custom_path);

    } else {

        mx_print_list(utils->env_vars);
    
    }
    free(flags);

    return 0;

}
