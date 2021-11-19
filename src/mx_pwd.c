#include "../inc/ush.h"

int mx_pwd(t_cmd_utils* utils)
{
    char *cwd = malloc(PATH_MAX * sizeof(char));

    if (getcwd(cwd, PATH_MAX))
    {
        mx_printstr(cwd);
        mx_printstr("\n");
    }
    else
    {
        perror("getcwd");
    }
    
    mx_strdel(&cwd);

    return 0;
}
