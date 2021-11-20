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

static void set_env_vars(t_cmd_utils* utils) {

    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if (mx_strstr(arg, "=") != NULL) {

            char* var_name = mx_strndup(arg, mx_get_char_index(arg, '='));
            char* var_value = mx_strchr(arg, '=') + 1;

            setenv(var_name, var_value, 1);
            mx_strdel(&var_name);

        }

    }
    mx_env_reset(&utils);
    print_env_vars(utils);

}

static void unset_env_var(t_cmd_utils** utils) {

    if (mx_strchr((*utils)->args[2], '=') != NULL) {
        mx_printerr(ENV_UNSET_ERR);
        mx_printerr((*utils)->args[2]);
        mx_printerr(ENV_INVARG_ERR);
        return;
    }

    unsetenv((*utils)->args[2]);
    mx_env_reset(utils);
    print_env_vars((*utils));

}

int mx_env(t_cmd_utils* utils) {

    if (utils->args[1] != NULL) {

        t_env_flags* flags = malloc(sizeof(*flags));
        mx_env_parse_flags(&flags, utils);

        if (flags->u) {

            unset_env_var(&utils);            

        }
        if (flags->i) {
            // clearenv();
            mx_clear_env_vars(&utils);
        }
        set_env_vars(utils);

    } else {

        print_env_vars(utils);
    
    }

    return 0;

}
