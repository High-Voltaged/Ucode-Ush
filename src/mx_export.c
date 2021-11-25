#include "../inc/ush.h"

bool export_cmp(char* a, char* b) {

    return mx_strcmp(a, b) > 0;

}

char* mx_get_var_name(const char* var) {

    char* var_name = mx_strndup(var, mx_get_char_index(var, '='));
    char* result = mx_replace_substr(var_name, "\"", "");
    mx_strdel(&var_name);
    return result;

} 

char* mx_get_var_value(const char* var) {

    if (mx_strstr(var, "=") != NULL) {

        char* var_value = mx_strchr(var, '=') + 1;
        char* result = mx_replace_substr(var_value, "\"", "");
        return result;
        
    }
    return mx_strdup("");

}

char* mx_get_var_str(t_env_var* env_var) {

    char* result = mx_strnew(mx_strlen(env_var->name) + mx_strlen(env_var->value) + 1);
    mx_strcat(result, env_var->name);
    mx_strcat(result, "=");
    mx_strcat(result, env_var->value);
    return result;

}

int mx_unset(t_cmd_utils* utils) {

    mx_parse_for_no_flags(utils, "unset");

    for (int idx = 1; utils->args[idx] != NULL; ++idx) {

        if (mx_remove_env_var(&utils, utils->args[idx]) != 0) {
            exit(1);
        }
        char* var_name = mx_get_var_name(utils->args[idx - 1]);
        unsetenv(var_name);
        mx_strdel(&var_name);

    }
    return 0;

}

void mx_export_vars(t_cmd_utils* utils) {

    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        char* var_name = mx_get_var_name(arg);
        t_env_var* exported_var = mx_find_env_var(utils->exported_vars, var_name, NULL);
        if (exported_var == NULL) {

            mx_env_push_back(&utils->exported_vars, arg);
            mx_env_push_back(&utils->env_vars, arg);

        } else if (mx_strstr(arg, "=") != NULL) {

            t_env_var* env_var = mx_find_env_var(utils->env_vars, var_name, NULL);
            char* var_value = mx_get_var_value(arg);
            if (var_value)
                setenv(var_name, var_value, 1);
            
            mx_overwrite_env_var(&exported_var, var_value);
            mx_overwrite_env_var(&env_var, var_value);
            mx_strdel(&var_value);
        
        } 
        mx_strdel(&var_name);
        
    }

}

int mx_export(t_cmd_utils* utils) {

    mx_parse_for_no_flags(utils, "export");
    if (utils->args[1] != NULL) {
        
        mx_export_vars(utils);

    } else {

        mx_print_env_list(utils->exported_vars, false);

    }
    return 0;

}

void mx_export_reset(t_cmd_utils** utils) {

    mx_env_clear_list(&(*utils)->exported_vars);
    for (int i = 0; environ[i] != NULL; ++i) {

        // remove the LS_COLORS check later
        if (environ[i][0] == '_' || mx_strstr(environ[i], "LS_COLORS") != NULL)
            continue;

        mx_env_push_back(&(*utils)->exported_vars, environ[i]);

    }
    mx_env_sort_list(&(*utils)->exported_vars, export_cmp);

}
