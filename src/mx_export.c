#include "../inc/ush.h"

bool export_cmp(void* a, void* b) {

    return mx_strcmp(a, b) > 0;

}

void mx_set_list_value(t_list** list, int idx, const char* new_value) {

    int i = 0;
    t_list** current = list;
    while ((*current) && (i != idx)) {
        (*current) = (*current)->next;
        ++i;
    }

    char* current_str = (*current)->data;
    mx_strdel(&current_str);
    (*current)->data = mx_strdup(new_value);
    
}

void mx_export_vars(t_cmd_utils* utils) {

    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];

        if (mx_strstr(arg, "=") != NULL) {

            char* var_name = mx_strndup(arg, mx_get_char_index(arg, '='));
            char* var_value = mx_strchr(arg, '=');
            var_value = var_value ? var_value + 1: "";

            if (var_value)
                setenv(var_name, var_value, 1);

            mx_strdel(&var_name);
            mx_env_reset(&utils);
            mx_export_reset(&utils);
        
        } else {

            char** exported_arr = mx_get_env_array(utils->exported_vars);
            int count = 0, temp = 0;
            for (; exported_arr[count] != NULL; ++count);
            int exported_idx = mx_binary_search(exported_arr, count, arg, &temp);
            if (exported_idx < 0) {

                mx_push_back(&utils->exported_vars, arg);

            }
            mx_del_strarr(&exported_arr);

        }
        
        // char** exported_arr = mx_get_env_array(utils->exported_vars);
        // printf("%s -- arr\n", exported_arr[71]);
        // int count = 0, temp = 0;
        // for (; exported_arr[count] != NULL; ++count);
        // int exported_idx = mx_binary_search(exported_arr, count, arg, &temp);
        // if (exported_idx < 0) {

        //     mx_push_back(&utils->exported_vars, arg);
        //     // mx_push_back(&utils->env_vars, arg);

        // } else if (is_to_set) {

        //     mx_set_list_value(&utils->exported_vars, exported_idx, arg);
        //     // mx_set_list_value(&utils->env_vars, exported_idx, arg);

        // }

        // mx_del_strarr(&exported_arr);

        // } else break;

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

    int i = 0;

    mx_clear_list(&(*utils)->exported_vars);
    
    t_list* env_var = (*utils)->env_vars;
    while (env_var) {

        char* env_str = env_var->data;
        if (env_str[0] == '_') {
            env_var = env_var->next;
            continue;
        }

        mx_push_back(&(*utils)->exported_vars, env_var->data);
        env_var = env_var->next;

    }
    mx_sort_list((*utils)->exported_vars, export_cmp);

}
