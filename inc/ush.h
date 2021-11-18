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
}              t_cmd_utils;

typedef int (*t_cmd_func)(t_cmd_utils* utils);



// 
char* mx_read_line();
// int mx_cd(t_cmd_utils* utils);
int mx_env(t_cmd_utils* utils);
int mx_exec(t_cmd_utils* utils);



// Array of function pointers for commands
static const t_cmd_func builtin_funcs[] = {
    NULL, &mx_env, NULL
    // &mx_cd, 
};

#endif
