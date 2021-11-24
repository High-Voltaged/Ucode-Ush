#include "../inc/ush.h"

bool export_cmp(void* a, void* b) {

    return mx_strcmp(a, b) > 0;

}

char* get_exported_name(const char* var) {

    char* var_name = mx_strndup(var, mx_get_char_index(var, '='));
    char* result = mx_replace_substr(var_name, "\"", "");
    mx_strdel(&var_name);
    return result;

} 

char* get_exported_value(const char* var) {

    if (mx_strstr(var, "=") != NULL) {

        char* var_value = mx_strchr(var, '=');
        bool is_empty = !mx_strcmp(var_value, "=") || !mx_strcmp(var_value, "=''");
        var_value = var_value && !is_empty ? var_value + 1 : mx_strdup("\'\'");
        char* result = mx_replace_substr(var_value, "\"", "");
        if (var_value && is_empty)
            mx_strdel(&var_value);
        return result;
        
    }
    return mx_strdup("\'\'");

} 

char* get_exported_var(const char* arg) {

    char* var_name = get_exported_name(arg);
    char* var_value = get_exported_value(arg);
    char* result = mx_strjoin(var_name, "=");
    mx_strcat(result, var_value);
    mx_strdel(&var_name);
    mx_strdel(&var_value);
    return result;

}

void mx_export_vars(t_cmd_utils* utils) {

    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if (mx_strstr(arg, "=") != NULL) {

            char* var_name = get_exported_name(arg);
            char* var_value = get_exported_value(arg);

            if (var_value)
                setenv(var_name, var_value, 1);
            mx_strdel(&var_name);
            mx_strdel(&var_value);
            mx_env_reset(&utils);
            mx_export_reset(&utils);
        
        } else {

            char* exported_var = get_exported_var(arg);
            char** exported_arr = mx_get_env_array(utils->exported_vars);
            int count = 0, temp = 0;
            for (; exported_arr[count] != NULL; ++count);
            int exported_idx = mx_binary_search(exported_arr, count, exported_var, &temp);
            if (exported_idx < 0) {

                mx_push_back(&utils->exported_vars, exported_var);

            }
            mx_del_strarr(&exported_arr);

        }
        
    }

}

int mx_export(t_cmd_utils* utils) {

    if (utils->args[1] != NULL) {
        
        mx_export_vars(utils);

    } else {

        mx_print_list(utils->exported_vars);

    }
    return 0;

}

void mx_export_reset(t_cmd_utils** utils) {

    mx_clear_list(&(*utils)->exported_vars);
    t_list* env_var = (*utils)->env_vars;
    while (env_var) {

        if (((char*)env_var->data)[0] == '_') {
            env_var = env_var->next;
            continue;
        }
        char* env_str = mx_strdup(env_var->data);
        char* var_value = get_exported_value(env_var->data);
        if (mx_strcmp(var_value, "\'\'") == 0) {
            mx_strcat(env_str, var_value);
        }

        mx_push_back(&(*utils)->exported_vars, env_str);
        env_var = env_var->next;

    }
    mx_sort_list((*utils)->exported_vars, export_cmp);

}
