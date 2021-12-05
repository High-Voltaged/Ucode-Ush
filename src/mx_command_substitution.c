#include "../inc/ush.h"

// char *mx_del_extra_newlines(const char *str)
// {
//     if (str == NULL)
//     {
//         return NULL;
//     }
    
//     char *tmp = mx_strtrim(str);

//     int x = 0;
//     for (int i = 0; tmp[i] != '\0'; i++)
//     {
//         if (!mx_isspace(tmp[i]))
//         {
//             tmp[x++] = tmp[i];
//         }
//         if (tmp[i] == '\n')
//         {
//             tmp[x++] = ' ';
//         }
//         if ((!mx_isspace(tmp[i])) && tmp[i + 1] == '\n')
//         {
//             tmp[x++] = ' ';
//         }
//     }
//     tmp[x] = '\0';
//     return tmp;
// }

void mx_command_substitution(char **args, t_cmd_utils* utils)
{
    int dollar_pos;
    char *to_replace = NULL;
    char *cmd = NULL;

    for (int i = 0; args[i] != NULL; i++)
    {
        if (args[i][0] == '\'' || mx_strlen(args[i]) <= 1)
        {
            continue;
        }

        char *tmp = args[i];

        while ((dollar_pos = mx_get_char_index(tmp, '$')) != -1)
        {
            if (mx_get_substr_index(&tmp[dollar_pos + 1], "$(") != -1)
            {
                tmp += dollar_pos + 1;
                continue;
            }
            else if (tmp[dollar_pos + 1] == '(')
            {
                
                //  echo $(ls $ dfgd $(echo $(pwd)) $(echo test))
                //  echo $(ls -l $(echo test) $(echo test1))

                cmd = mx_strndup(&tmp[dollar_pos + 2], mx_get_char_index(&tmp[dollar_pos + 2], ')'));

                char** cmd_args = NULL;
                mx_parse_line(utils, cmd, &cmd_args);

                char* result = mx_cmd_exec(utils, cmd_args);

                if (args[i][0] != '\"')
                {
                    result = mx_replace_substr_free(result, "\n", " ");///////
                    // result = mx_strtrim(result);
                    //?????????????
                }
                
                to_replace = mx_strndup(&tmp[dollar_pos], mx_strlen(cmd) + 3);

                args[i] = mx_replace_substr_free(args[i], to_replace, result);
                
                mx_strdel(&cmd);
                tmp = args[i];
            }
        }
    }
}
