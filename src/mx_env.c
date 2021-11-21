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

void mx_print_env_vars(t_cmd_utils* utils) {

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
    // mx_print_env_vars((*utils));

}

int mx_env(t_cmd_utils* utils) {

    int curr_arg_idx = 1;
    t_env_flags* flags = malloc(sizeof(*flags));
    if (utils->args[1] != NULL) {

        mx_env_parse_flags(&flags, utils, &curr_arg_idx);

        if (flags->u) {
            unset_env_var(&utils, &curr_arg_idx);            
        }
        
        const char* custom_path = flags->P ? mx_strdup(utils->args[curr_arg_idx++]) : NULL;
        if (flags->i) {

            mx_clear_env_vars(&utils);
            set_env_vars(utils, &curr_arg_idx);
            exec_env_utility(utils, utils->args[curr_arg_idx], curr_arg_idx + 1, custom_path);
            mx_env_reset(&utils);
        
        } else {
            set_env_vars(utils, &curr_arg_idx);
            exec_env_utility(utils, utils->args[curr_arg_idx], curr_arg_idx + 1, custom_path);
            mx_env_reset(&utils);
        }

    } 
    if ((!flags->i && !flags->P) || utils->args[curr_arg_idx] == NULL) {

        mx_print_env_vars(utils);
    
    }
    free(flags);

    return 0;

}
