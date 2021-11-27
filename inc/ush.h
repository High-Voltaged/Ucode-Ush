#ifndef USH_H
#define USH_H

#include "../libmx/inc/libmx.h"
#include "const.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

extern char** environ;

typedef struct s_env_var {
    char* name;
    char* value;
    int index;
    struct s_env_var* next;
}              t_env_var;

typedef struct s_cmd_utils {
    char** args;
    t_env_var* env_vars;
    t_env_var* exported_vars;
    // int status;
}              t_cmd_utils;

typedef int (*t_cmd_func)(t_cmd_utils* utils);


// COMMANDS' FLAGS
typedef struct s_cd_flags {
    int s, P, prev;
}              t_cd_flags;

typedef struct s_which_flags {
    int s, a;
}              t_wch_flags;

typedef struct s_env_flags {
    int i, P, u;
    char* u_param;
    char* p_param;
}              t_env_flags;

typedef struct s_echo_flags {
    int n, e, E;
}              t_echo_flags;

typedef struct s_pwd_flags {
    int L, P;
}              t_pwd_flags;

// PARSING

char* mx_read_line();
void mx_parse_line(t_cmd_utils *utils, char *line);
int mx_cd_parse_flags(t_cd_flags** flags, t_cmd_utils* utils);
int mx_wch_parse_flags(t_wch_flags** flags, t_cmd_utils* utils);
int mx_env_parse_flags(t_env_flags** flags, t_cmd_utils* utils, int* arg_idx);
void mx_echo_parse_flags(t_echo_flags** flags, t_cmd_utils* utils, int *flag_count);
int mx_parse_for_no_flags(t_cmd_utils* utils, const char* cmd);
int mx_pwd_parse_flags(t_pwd_flags** flags, t_cmd_utils* utils);
void mx_cd_add_flag(t_cd_flags** flags, char flag);
void mx_wch_add_flag(t_wch_flags** flags, char flag);
void mx_env_add_flag(t_env_flags** flags, char flag);
void mx_echo_add_flag(t_echo_flags** flags, char flag);
void mx_pwd_add_flag(t_pwd_flags **flags, char flag);
char *mx_normalize_path(char *pwd, char *point);

// COMMAND EXECUTION

int mx_exec(t_cmd_utils* utils);
int mx_cd(t_cmd_utils* utils);
int mx_env(t_cmd_utils* utils);
int mx_echo(t_cmd_utils* utils);
int mx_which(t_cmd_utils* utils);
int mx_pwd(t_cmd_utils* utils);
int mx_export(t_cmd_utils* utils);
int mx_unset(t_cmd_utils* utils);
int mx_exit(t_cmd_utils* utils);

// ERROR HANDLING

void mx_print_odd_quotes_err();
void mx_print_too_many_args_err(const char* cmd);
void mx_print_cmd_err(const char* cmd, const char* error);
void mx_print_flag_err(const char* cmd, char flag);
void mx_print_env_option_err(char flag, const char* cmd);
void mx_print_env_error(const char* error, const char* env_util);
void mx_print_env_arg_err(char flag);

// ENV COMMAND UTILS

void mx_env_reset(t_cmd_utils** utils);
void mx_set_env_vars(t_cmd_utils* utils, int* arg_idx);
int mx_remove_env_var(t_cmd_utils** utils, char* name);
char** mx_get_env_util_args(t_cmd_utils* utils, int util_arg_idx);
int exec_env_utility(t_cmd_utils* utils, int util_arg_idx, t_env_flags* flags);
char** mx_get_exec_paths(const char* to_find, const char* custom_path, bool single_search);

void mx_export_reset(t_cmd_utils** utils);
char* mx_get_var_str(t_env_var* env_var);

t_env_var *mx_create_env_var(char* env_var);
void mx_env_push_back(t_env_var **list, char* env_var);
void mx_env_pop_index(t_env_var **list, int index);
void mx_env_clear_list(t_env_var **list);
int mx_env_list_size(t_env_var* list);
void mx_print_env_list(t_env_var* list, bool is_env_cmd);
void mx_env_sort_list(t_env_var** list, bool (*cmp)(char* a, char* b));
t_env_var* mx_find_env_var(t_env_var* list, char* name, int* index);
void mx_overwrite_env_var(t_env_var** env_var, const char* value);
char* mx_get_var_name(const char* var);
char* mx_get_var_value(const char* var);
char** mx_get_env_array(t_env_var* list);


// MISC UTILS

char* get_dir_path(char* dir, const char* file_name);
int mx_util_arg_count(t_cmd_utils* utils);


// Array of function pointers for commands
static const t_cmd_func builtin_funcs[] = {
    &mx_cd, &mx_env, &mx_echo, &mx_pwd, &mx_which, &mx_export, &mx_unset, &mx_exit, NULL 
};

#endif
