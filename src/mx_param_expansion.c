#include "../inc/ush.h"

static int get_param_len(char *str, int dollar_pos)
{
    int space_idx = mx_get_char_index(&str[dollar_pos + 1], ' ');
    int quote_idx = mx_get_char_index(&str[dollar_pos + 1], '\"');

    if (space_idx == -1 && quote_idx == -1)
    {
        return -1;
    }

    if (space_idx != -1 && quote_idx == -1)
    {
        return space_idx;
    }

    if (space_idx == -1 && quote_idx != -1)
    {
        return quote_idx;
    }

    if (space_idx != -1 && quote_idx != -1)
    {
        return space_idx < quote_idx ? space_idx : quote_idx;
    }
    
    return -1;
}

void mx_param_expansion(char **args)
{
    int dollar_pos;
    int to_replace_len;
    char *to_replace = NULL;
    char *param = NULL;
    char *env_var = NULL;

    for (int i = 0; args[i] != NULL; i++)
    {
        if (args[i][0] == '\'')
        {
            continue;
        }
        
        char *dup_arg = mx_strdup(args[i]);
        char *tmp = dup_arg;

        while ((dollar_pos = mx_get_char_index(tmp, '$')) != -1)
        {
            if (dollar_pos != -1 && !mx_isspace(tmp[dollar_pos + 1]) && tmp[dollar_pos + 1] != '(' && tmp[dollar_pos + 1] != '\\')
            {
                if (tmp[dollar_pos + 1] == '{')
                {
                    param = mx_strndup(&tmp[dollar_pos + 2], mx_get_char_index(&tmp[dollar_pos + 2], '}'));
                    to_replace_len = mx_strlen(param) + 3;
                }
                else
                {
                    int param_len = get_param_len(tmp, dollar_pos);
                    param = (param_len != -1) ? mx_strndup(&tmp[dollar_pos + 1], param_len) : mx_strdup(&tmp[dollar_pos + 1]);
                    to_replace_len = mx_strlen(param) + 1;   
                }

                env_var = getenv(param);
                to_replace = mx_strndup(&tmp[dollar_pos], to_replace_len);

                if (env_var != NULL)
                {
                    args[i] = mx_replace_substr_free(args[i], to_replace, env_var);
                }
                else
                {
                    args[i] = mx_replace_substr_free(args[i], to_replace, "");
                }

                mx_strdel(&param);
                mx_strdel(&to_replace);
            }

            tmp += dollar_pos + 1;// if str includes single $ 
        }
        mx_strdel(&dup_arg);
    }
}
