#ifndef USH_H
#define USH_H

#include "../libmx/inc/libmx.h"
#include "const.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

extern char** environ;

typedef struct s_cmd_utils {
    char* cmd;          // parsed command
    char** args;        // arguments for the command 
    char* env_vars[100]; // saved enviroment variables
    // int status;
}              t_cmd_utils;

typedef int (*t_cmd_func)(t_cmd_utils* utils);


// COMMANDS' FLAGS
typedef struct s_cd_flags {
    int s, P, prev;
}              t_cd_flags;


// 
char* mx_read_line();
int mx_exec(t_cmd_utils* utils);
int mx_cd(t_cmd_utils* utils);
int mx_env(t_cmd_utils* utils);
int mx_exit(t_cmd_utils* utils);

void mx_env_reset(t_cmd_utils** utils);

bool mx_is_flag_found(char flags[], char flag);

// Array of function pointers for commands
static const t_cmd_func builtin_funcs[] = {
    &mx_cd, &mx_env, &mx_exit, NULL 
};

#endif
