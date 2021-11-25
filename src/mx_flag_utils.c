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
                    mx_print_option_err(arg[j], "cd");
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
                    mx_print_option_err(arg[j], "which");
                    // exit(1);
                }
            
            }

        } else break;
    }

}

int mx_env_parse_flags(t_env_flags** flags, t_cmd_utils* utils, int* arg_idx) {

    (*flags)->i = (*flags)->P = (*flags)->u = 0;
    (*flags)->u_param = (*flags)->p_param = NULL;

    if (utils->args == NULL) return 0;

    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if (mx_strstr(arg, "=") == NULL) {
            if ((arg[0] == '-') && mx_isalpha(arg[1])) {

                ++(*arg_idx);
                int flag_count = mx_strlen(arg);
                for (int j = 1; j < flag_count; j++) {

                    if (arg[j] == 'u') {
                        
                        mx_env_add_flag(flags, arg[j]);
                        if (utils->args[i + 1] != NULL && j == flag_count - 1) {
                            (*flags)->u_param = mx_strdup(utils->args[i + 1]);
                            ++(*arg_idx);
                        } else if (j == flag_count - 1) {
                            mx_print_env_arg_err(arg[j]);
                        }
                    
                    } else if (arg[j] == 'P') {
                        
                        mx_env_add_flag(flags, arg[j]);
                        if (utils->args[i + 1] != NULL && j == flag_count - 1) {
                            (*flags)->p_param = mx_strdup(utils->args[i + 1]);
                            ++(*arg_idx);
                        } else if (j == flag_count - 1) {
                            mx_print_env_arg_err(arg[j]);
                        }
                    
                    } else if (arg[j] == 'i') {
                       
                        mx_env_add_flag(flags, arg[j]);
                   
                    } else {
                        mx_print_option_err(arg[j], "env");
                        --(*arg_idx);
                        return 1;
                    }
                }
            } 
        } else break;
    }
    return 0;
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

void mx_pwd_parse_flags(t_pwd_flags** flags, t_cmd_utils* utils) {

    (*flags)->L = 1;
    (*flags)->P = 0;

    if (utils->args == NULL) return;

    char* const_flags = "LP"; //const
    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if ((arg[0] == '-') && !mx_isspace(arg[1])) {

            for (int j = 1; arg[j] != '\0'; j++) {

                if (mx_is_flag_found(const_flags, arg[j])) {
                    mx_pwd_add_flag(flags, arg[j]);
                } else {
                    mx_print_flag_err("pwd", arg[j]);
                    // exit(1);
                }
            
            }

        } else break;
    }

}

void mx_parse_for_no_flags(t_cmd_utils* utils, const char* cmd) {

    if (utils->args == NULL) return;

    for (int i = 1; utils->args[i] != NULL; ++i) {

        char* arg = utils->args[i];
        if ((arg[0] == '-') && mx_isalpha(arg[1])) {

            mx_print_option_err(arg[1], cmd);
            // print usage

        } else break;
    }

}
