#ifndef CONST_H
#define CONST_H

#define PATH_MAX 4096

static const char* builtin_cmds[] = {
    "cd", "env", "pwd", "exit", NULL
};

static const char* WHICH_STR = "which";

static const char* OLDPWD_STR = "OLDPWD";
static const char* PWD_STR = "PWD";
static const char* HOME_STR = "HOME";
static const char* PATH_STR = "PATH";

static const char* USH_STR = "ush: ";

#endif
