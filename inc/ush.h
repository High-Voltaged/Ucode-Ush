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


// PARSING

char* mx_read_line();
void mx_parse_line(t_cmd_utils *utils, char *line);
void mx_cd_parse_flags(t_cd_flags** flags, t_cmd_utils* utils);
void mx_wch_parse_flags(t_wch_flags** flags, t_cmd_utils* utils);
void mx_cd_add_flag(t_cd_flags** flags, char flag);
void mx_wch_add_flag(t_wch_flags** flags, char flag);

// COMMAND EXECUTION

int mx_exec(t_cmd_utils* utils);
int mx_cd(t_cmd_utils* utils);
int mx_env(t_cmd_utils* utils);
int mx_which(t_cmd_utils* utils, char* to_find);
int mx_exit(t_cmd_utils* utils);

// ERROR HANDLING

void mx_print_flag_err(char flag);

// MISC UTILS

void mx_env_reset(t_cmd_utils** utils);
bool mx_is_flag_found(char flags[], char flag);

// Array of function pointers for commands
static const t_cmd_func builtin_funcs[] = {
    &mx_cd, &mx_env, &mx_exit, NULL 
};

#endif