#ifndef USH_H
#define USH_H

#include "../libmx/inc/libmx.h"
#include "const.h"
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

extern char** environ;

typedef struct s_cmd_utils {
    char** args;        // arguments for the command 
    char* env_vars[100]; // saved enviroment variables
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
}              t_env_flags;

typedef struct s_echo_flags {
    int n, e, E;
}              t_echo_flags;

// PARSING

char* mx_read_line();
void mx_parse_line(t_cmd_utils *utils, char *line);
void mx_cd_parse_flags(t_cd_flags** flags, t_cmd_utils* utils);
void mx_wch_parse_flags(t_wch_flags** flags, t_cmd_utils* utils);
void mx_env_parse_flags(t_env_flags** flags, t_cmd_utils* utils, int* arg_idx);
void mx_echo_parse_flags(t_echo_flags** flags, t_cmd_utils* utils, int *flag_count);
void mx_cd_add_flag(t_cd_flags** flags, char flag);
void mx_wch_add_flag(t_wch_flags** flags, char flag);
void mx_env_add_flag(t_env_flags** flags, char flag);
void mx_echo_add_flag(t_echo_flags** flags, char flag);

// COMMAND EXECUTION

int mx_exec(t_cmd_utils* utils);
int mx_cd(t_cmd_utils* utils);
int mx_env(t_cmd_utils* utils);
int mx_echo(t_cmd_utils* utils);
int mx_which(t_cmd_utils* utils, const char* to_find);
int mx_pwd(t_cmd_utils* utils);
int mx_exit(t_cmd_utils* utils);

// ERROR HANDLING

void mx_print_flag_err(char flag);
void mx_print_env_error(char flag);

// MISC UTILS

char** mx_get_env_util_args(t_cmd_utils* utils, int util_arg_idx);
int exec_env_utility(t_cmd_utils* utils, const char* env_util, int util_arg_idx, const char* custom_path);
char** mx_get_exec_paths(const char* to_find, const char* custom_path, bool single_search);
void mx_print_env_vars(t_cmd_utils* utils);
char* get_dir_path(char* dir, const char* file_name);
void mx_environ_reset(t_cmd_utils* utils);
void mx_env_reset(t_cmd_utils** utils);
void mx_clear_env_vars(t_cmd_utils** utils);
bool mx_is_flag_found(char flags[], char flag);

// Array of function pointers for commands
static const t_cmd_func builtin_funcs[] = {
    &mx_cd, &mx_env, &mx_echo, &mx_pwd, &mx_exit, NULL 
};

#endif
