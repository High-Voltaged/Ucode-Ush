#include "../inc/ush.h"

bool mx_is_flag_found(char* flags, char flag) {

    for (int i = 0; i < mx_strlen(flags); ++i) {

        if (flag == flags[i])
            return true;

    }
    return false;

}

static bool is_only_flags(char* flags, char* flags_str) {

    bool is_flag;
    for (int i = 1; i < mx_strlen(flags_str); i++) {
        is_flag = false;

        for (int j = 0; j < mx_strlen(flags); j++) {

            if (flags_str[i] == flags[j]) {
                is_flag = true;
                break;
            }
        }

        if (!is_flag)
        {
            return false;
        }
        
    }
    return true;
}

void mx_cd_parse_flags(t_cd_flags** flags, t_cmd_utils* utils) {

    (*flags)->P = (*flags)->prev = (*flags)->s = 0;

    if (utils->args == NULL) return;

    char* const_flags = "sP";
    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if (arg[0] == '-' && mx_strlen(arg) == 1) {
            mx_cd_add_flag(flags, arg[0]);
            continue;
        } 
        if ((arg[0] == '-') && mx_isalpha(arg[1])) {

            for (int j = 1; arg[j] != '\0'; j++) {

                if (mx_is_flag_found(const_flags, arg[j])) {
                    mx_cd_add_flag(flags, arg[j]);
                } else {
                    mx_print_flag_err(arg[j]);
                    // exit(1);
                }
            
            }

        } else break;
    }

}

void mx_wch_parse_flags(t_wch_flags** flags, t_cmd_utils* utils) {

    (*flags)->s = (*flags)->a = 0;

    if (utils->args == NULL) return;

    char* const_flags = "sa";
    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if ((arg[0] == '-') && mx_isalpha(arg[1])) {

            for (int j = 1; arg[j] != '\0'; j++) {

                if (mx_is_flag_found(const_flags, arg[j])) {
                    mx_wch_add_flag(flags, arg[j]);
                } else {
                    mx_print_flag_err(arg[j]);
                    // exit(1);
                }
            
            }

        } else break;
    }

}

void mx_env_parse_flags(t_env_flags** flags, t_cmd_utils* utils, int* arg_idx) {

    (*flags)->i = (*flags)->P = (*flags)->u = 0;

    if (utils->args == NULL) return;

    char* const_flags = "iPu";
    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if ((arg[0] == '-') && mx_isalpha(arg[1])) {

            for (int j = 1; arg[j] != '\0'; j++) {

                if (mx_is_flag_found(const_flags, arg[j])) {
                    
                    if ((arg[j] == 'u' || arg[j] == 'P') && utils->args[i + 1] == NULL) {
                        mx_print_env_error(arg[j]);
                        continue; // possibly not continue
                    }
                    
                    mx_env_add_flag(flags, arg[j]);
                } else {
                    mx_print_flag_err(arg[j]);
                    // exit(1);
                }
            
            }

        } else break;
        ++(*arg_idx);
    
    }

}

void mx_echo_parse_flags(t_echo_flags** flags, t_cmd_utils* utils, int *flag_count) {

    (*flags)->n = (*flags)->E = 0;
    (*flags)->e = 1;
    // (*flags)->E = 1; //default

    if (utils->args == NULL) return;

    char* const_flags = "neE"; //const
    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if ((arg[0] == '-') && mx_isalpha(arg[1])) {

            if (!is_only_flags(const_flags, arg))
                return;
            
            for (int j = 1; arg[j] != '\0'; j++)
                mx_echo_add_flag(flags, arg[j]);

        } else return;

        (*flag_count)++;
    }

}
