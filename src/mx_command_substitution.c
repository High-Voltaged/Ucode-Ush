#include "../inc/ush.h"

void mx_command_substitution(char **args, t_cmd_utils* utils)
{
    // FILE* fd = fopen("test.txt", "w");
    int dollar_pos;
    char *to_replace = NULL;
    char *cmd = NULL;

    for (int i = 0; args[i] != NULL; i++)
    {
        if (args[i][0] == '\'')
        {
            continue;
        }
        // char *dup_arg = mx_strdup(args[i]);
        // char *tmp = dup_arg;
        char *tmp = args[i];

        while ((dollar_pos = mx_get_char_index(tmp, '$')) != -1)
        {
                // printf("tmp = '%s'\n", tmp);

            if (mx_get_substr_index(&tmp[dollar_pos + 1], "$(") != -1)
            {
                tmp += dollar_pos + 1;
                continue;
            }
            else if (tmp[dollar_pos + 1] == '(')
            {
                
                //  echo $(ls $  $(ls $(pwd)))

                // int close_parenthesis_idx = mx_get_char_index(&tmp[dollar_pos + 2], ')');
                // printf("tmp = '%s'\n", tmp);
                cmd = mx_strndup(&tmp[dollar_pos + 2], mx_get_char_index(&tmp[dollar_pos + 2], ')'));
                // printf("cmd = '%s'\n", cmd);

                char** cmd_args = mx_strsplit(cmd, ' ');
                
                char* result = mx_cmd_exec(utils, cmd_args);

                // printf("result = '%s'\n", result);
                to_replace = mx_strndup(&tmp[dollar_pos], mx_strlen(cmd) + 3);
                // printf("to_replace = '%s'\n", to_replace);


                args[i] = mx_replace_substr_free(args[i], to_replace, result);
                // execvp, mx_run_subshell
                
                mx_strdel(&cmd);
                // mx_strdel(&to_replace);
                tmp = args[i];
            }

            // tmp += dollar_pos + 1;
        }
        // mx_strdel(&dup_arg);
    }
    
    mx_exec(utils);
}
