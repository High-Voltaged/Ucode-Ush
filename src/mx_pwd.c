#include "../inc/ush.h"

int mx_pwd(t_cmd_utils* utils)
{
    char *cwd = NULL;

    // if (getcwd(cwd, PATH_MAX))
    if ((cwd = getenv(PWD_STR)))
    {
        mx_printstr(cwd);
        mx_printstr("\n");
    }
    else
    {
        perror("getcwd");
    }
    
    // mx_strdel(&cwd);

    return 0;
}
