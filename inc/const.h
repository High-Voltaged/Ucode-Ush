#ifndef CONST_H
#define CONST_H

#define PATH_MAX 4096

static const char* builtin_cmds[] = {
    "cd", "env", "echo", "pwd", "which", "export", "exit", NULL
};

static const char* WHICH_STR = "which";

static const char* OLDPWD_STR = "OLDPWD";
static const char* PWD_STR = "PWD";
static const char* HOME_STR = "HOME";
static const char* PATH_STR = "PATH";

static const char* PROMPT_STR = "u$h> ";
static const char* USH_STR = "ush: ";
static const char* BUILTIN_CMD_ERR = ": shell built-in command\n";
static const char* ENV_ARG_ERR = "env: option requires an argument -- '";
static const char* ENV_UNSET_ERR = "env: cannot unset '";
static const char* ENV_INVARG_ERR = "': Invalid argument\n";

#endif
