#include "../inc/ush.h"

// bool mx_isdelim(char c, char *delim)
// {
//     if (!c || !delim)
//         return false;

//     for (int i = 0; delim[i]; i++)
//     {
//         if (delim[i] == c)
//             return true;
//     }

//     return false;
// }

// "sdfsdfsd sdfsdfs" - del quotes and join, 'dsfs' - del quotes
// static int check_odd_quotes(char *str)
// {
//     char *quotes[] = {"'", "\"", "`", NULL};
//     char *b_quotes[] = {"\\'", "\\\"", "\\`", NULL};

//     for (int i = 0; quotes[i] != NULL; i++)
//     {
//         if ((mx_count_substr(str, quotes[i]) - mx_count_substr(str, b_quotes[i])) % 2 != 0)
//         {
//             mx_print_odd_quotes_err();
//             return 1;
//         }
//     }

//     return 0;
// }

static int check_odd_quotes(char *str)
{
    int single_quotes = 0;  //'
    int double_quotes = 0;  //"
    int parentheses = 0;    //()

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!single_quotes && !double_quotes)
        {
            if (str[i] == '\'' && str[i - 1] != '\\')
            {
                single_quotes++;
            }
            else if (str[i] == '\"' && str[i - 1] != '\\')
            {
                double_quotes++;
            }
            else if (str[i] == '(')
            {
                parentheses++;
            }
            else if (str[i] == ')')
            {
                parentheses--;
                if (parentheses < 0)
                {
                    mx_printerr("ush: parse error near `)\'\n");
                    return 1;
                }  
            }
        }
        else if (single_quotes && !double_quotes)
        {
            if (str[i] == '\'' && str[i - 1] != '\\')
            {
                single_quotes--;
            }
            continue;
        }
        else if (!single_quotes && double_quotes)
        {
            if (str[i] == '\"' && str[i - 1] != '\\')
            {
                double_quotes--;
            }
            continue;
        }
    }
    
    if (single_quotes || double_quotes || parentheses)
    {
        mx_print_odd_quotes_err();
        return 1;
    }

    return 0;
}

char *mx_replace_substr_free(char *str, char *sub, char *replace)
{
    char *tmp = str;
    char *result = mx_replace_substr(tmp, sub, replace);
    mx_strdel(&tmp);

    return result;
}

static void handle_backslashes(char **args)
{
    if (!mx_strcmp(args[0], "echo") || !mx_strcmp(args[0], "\"echo\"") || !mx_strcmp(args[0], "'echo'"))
    {
        return;
    }

    for (int i = 1; args[i] != NULL; i++)
    {
        if (args[i][0] != '\'' && args[i][0] != '"')
        {
            args[i] = mx_replace_substr_free(args[i], "\\ ", " ");
            args[i] = mx_replace_substr_free(args[i], "\\`", "`");
            args[i] = mx_replace_substr_free(args[i], "\\'", "'");
            args[i] = mx_replace_substr_free(args[i], "\\\"", "\"");
            args[i] = mx_replace_substr_free(args[i], "\\", "");
        }
    }
}

static void del_extra_quotes(char **args)
{
    for (size_t i = 0; args[i] != NULL; i++)
    {
        if (args[i][0] == '\'' || args[i][0] == '"')
        {
            char *tmp = mx_strndup(args[i] + 1, mx_strlen(args[i]) - 2); //dup without quotes
            mx_strdel(&args[i]);
            args[i] = tmp;
        }

        if (i == 0 && !mx_strcmp(args[0], "echo"))
        {
            return;
        }
    }
}

int get_close_parenthesis_idx(char *str)
{
    int count_parenthesis = 1;

    for (int i = 2; str[i] != '\0'; i++)
    {
        if (str[i] == '$' && str[i + 1] == '(')
        {
            count_parenthesis++;
        }
        if (str[i] == ')')
        {
            count_parenthesis--;
        }
        if (count_parenthesis == 0)
        {
            return i;
        }
    }

    return -1;
}

int mx_parse_line(t_cmd_utils *utils, char *line)
{
    char *mod_line = mx_strtrim(line);
    if (check_odd_quotes(mod_line) != 0)
        return 1;

    utils->args = NULL;

    //for line with only func name without args
    // if ((mx_count_words(mod_line, ' ') - mx_count_substr(mod_line, "\\ ")) <= 1)
    // {
    //     utils->args = malloc(2 * sizeof(char *));
    //     utils->args[0] = mx_strndup(mod_line, mx_strlen(mod_line));
    //     utils->args[1] = NULL;
    //     // handle_backslashes(utils->args);
    //     del_extra_quotes(utils->args);
    //     return 0;
    // }
    //

    char *tmp = mod_line;
    int space_index = 0;
    int i;

    for (i = 0; space_index != -1; i++)
    {
        space_index = mx_get_char_index(tmp, ' ');
        int backslash_index = mx_get_substr_index(tmp, "\\ ");

        while (space_index == backslash_index + 1 && backslash_index != -1)
        {
            tmp += space_index + 1;
            space_index = mx_get_char_index(tmp, ' ');
            backslash_index = mx_get_substr_index(tmp, "\\ ");
        }

        utils->args = mx_realloc(utils->args, (i + 2) * sizeof(char *));

        if (space_index == -1)
        {
            utils->args[i] = mx_strndup(mod_line, mx_strlen(mod_line));
        }
        else if (mod_line[0] == '"')
        {
            utils->args[i] = mx_strndup(mod_line, mx_get_char_index(mod_line + 1, '\"') + 2);
            tmp += mx_get_char_index(mod_line + 1, '\"') + 2;
        }
        else if (mod_line[0] == '\'')
        {
            utils->args[i] = mx_strndup(mod_line, mx_get_char_index(mod_line + 1, '\'') + 2);
            tmp += mx_get_char_index(mod_line + 1, '\'') + 2;
        }
        else if (mod_line[0] == '$' && mod_line[1] == '(')
        {
            int close_parenthesis_idx = get_close_parenthesis_idx(mod_line);
            // if (close_parenthesis_idx == -1)
            // {
            //     mx_printerr("Odd number of quotes)))))))))))).\n");
                
            // }
            
            utils->args[i] = mx_strndup(mod_line, close_parenthesis_idx + 1);
            tmp += close_parenthesis_idx + 1;
        }
        else
        {
            tmp += space_index + 1;
            utils->args[i] = mx_strndup(mod_line, mx_strlen(mod_line) - mx_strlen(tmp) - 1);
        }

        while (mx_isspace(tmp[0]))
        {
            tmp++;
        }

        mod_line = tmp;
    }

    utils->args[i] = NULL;

    handle_backslashes(utils->args);
    mx_param_expansion(utils->args);  

    if (mx_tilde_expansion(utils->args) != 0)
        return 1;

    mx_command_substitution(utils->args, utils);
    del_extra_quotes(utils->args);

    return 0;
}
