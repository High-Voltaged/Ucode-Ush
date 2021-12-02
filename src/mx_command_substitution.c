#include "../inc/ush.h"

void mx_command_substitution(char **args)
{
    FILE* fd = fopen("test.txt", "w");
    int dollar_pos;
    char *to_replace = NULL;
    char *cmd = NULL;

    for (int i = 0; args[i] != NULL; i++)
    {
        char *dup_arg = mx_strdup(args[i]);
        char *tmp = dup_arg;

        while ((dollar_pos = mx_get_char_index(tmp, '$')) != -1)
        {
            if (!mx_isspace(tmp[dollar_pos + 1]) && tmp[dollar_pos + 1] == '(')
            {
                cmd = mx_strndup(&tmp[dollar_pos + 2], mx_get_char_index(&tmp[dollar_pos + 2], ')'));
                fprintf(fd, "test here\n");
                // execvp, mx_run_subshell

                // if (env_var != NULL)
                // {
                //     to_replace = mx_strndup(&tmp[dollar_pos], mx_strlen(cmd) + 3);
                //     args[i] = mx_replace_substr_free(args[i], to_replace, env_var);
                // }
                // else
                // {
                //     to_replace = mx_strndup(&tmp[dollar_pos], mx_strlen(cmd) + 3);
                //     args[i] = mx_replace_substr_free(args[i], to_replace, "");
                // }

                mx_strdel(&cmd);
                // mx_strdel(&to_replace);
            }

            tmp += dollar_pos + 1;
        }
        mx_strdel(&dup_arg);
    }
    fclose(fd);
}
