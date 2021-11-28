#include "../inc/ush.h"

void mx_param_expansion(char **args)
{
    int dollar_pos;
    char *to_replace = NULL;
    char *param = NULL;
    char *env_var = NULL;

    for (int i = 0; args[i] != NULL; i++)
    {
        dollar_pos = mx_get_char_index(args[i], '$');

        if (dollar_pos != -1 && !mx_isspace(args[i][dollar_pos + 1]) && args[i][dollar_pos + 1] != '\\')
        {
            if (args[i][dollar_pos + 1] == '{')
            {
                param = mx_strndup(&args[i][dollar_pos + 2], mx_get_char_index(&args[i][dollar_pos + 2], '}'));
            }
            else
            {
                int space_idx = mx_get_char_index(&args[i][dollar_pos + 1], ' ');
                param = (space_idx != -1) ? mx_strndup(&args[i][dollar_pos + 2], space_idx + 1) : mx_strdup(&args[i][dollar_pos + 1]);   
            }

            env_var = getenv(param);

            if (env_var != NULL)
            {
                to_replace = mx_strndup(&args[i][dollar_pos], mx_strlen(param) + 3);
                args[i] = mx_replace_substr_free(args[i], to_replace, env_var);
            }
            else
            {
                to_replace = mx_strndup(&args[i][dollar_pos], mx_strlen(param) + 3);
                args[i] = mx_replace_substr_free(args[i], to_replace, "");
            }

            mx_strdel(&param);
            mx_strdel(&to_replace);
        }
    }
}
