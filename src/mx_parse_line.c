#include "../inc/ush.h"

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

static void move_to_space(char *str, int *index, bool *is_finish)
{   
    while (str[*index] != '\0' && (!mx_isspace(str[*index]) || (str[(*index) - 1] == '\\' && str[(*index)] == ' ')))
    {
        if (str[*index] == '\'')
        {
            int single_quotes_idx = mx_get_char_index(&str[(*index)], '\'');
            int backslash_index = mx_get_substr_index(&str[(*index)], "\\\'");
                // printf("(%d)%d\n",single_quotes_idx, backslash_index);
            while (single_quotes_idx == backslash_index + 1 && backslash_index != -1)
            {
                *index += single_quotes_idx + 1;
                single_quotes_idx = mx_get_char_index(&str[*index], '\'');
                backslash_index = mx_get_substr_index(&str[*index], "\\\'");
            }
            *index += single_quotes_idx + 1;
            // *index = mx_get_char_index(&str[(*index) + 1], '\'') + 2;
            continue;
        }
        else if (str[*index] == '\"')
        {
            int double_quotes_idx = mx_get_char_index(&str[(*index)], '\"');
            int backslash_index = mx_get_substr_index(&str[(*index)], "\\\"");

            while (double_quotes_idx == backslash_index + 1 && backslash_index != -1)
            {
                *index += double_quotes_idx + 1;
                double_quotes_idx = mx_get_char_index(&str[*index], '\"');
                backslash_index = mx_get_substr_index(&str[*index], "\\\"");
            }
            *index += double_quotes_idx + 1;
            // *index = mx_get_char_index(&str[(*index) + 1], '\"') + 2;
            continue;
        }
        else if (str[*index] == '$' && str[(*index) + 1] == '(')
        {
            *index += get_close_parenthesis_idx(&str[*index]);
            continue;
        }
        else
        {
            (*index)++;
        }        
    }

    if (str[*index] == '\0')
    {
        *is_finish = true;
    }
    else
    {
        *is_finish = false;
    }
}

int mx_parse_line(t_cmd_utils *utils, char *line, char ***dst)
{
    char *mod_line = mx_strtrim(line);
    if (check_odd_quotes(mod_line) != 0)
        return 1;

    (*dst) = NULL;

    char *tmp = mod_line;
    // int space_index = 0;
    int i = 0;
    bool is_finish = false;

    while (!is_finish)
    {
        int index = 0;

        (*dst) = mx_realloc((*dst), (i + 2) * sizeof(char *));

        move_to_space(tmp, &index, &is_finish);

        if (is_finish)
        {
            (*dst)[i++] = mx_strndup(mod_line, mx_strlen(mod_line));
        }
        else
        {
            tmp += index;
            (*dst)[i++] = mx_strndup(mod_line, mx_strlen(mod_line) - mx_strlen(tmp));
        }

        while (mx_isspace(tmp[0]))
        {
            tmp++;
        }

        mod_line = tmp;
    }

    (*dst)[i] = NULL;

    handle_backslashes((*dst));
    mx_param_expansion((*dst));  

    if (mx_tilde_expansion((*dst)) != 0)
        return 1;

    mx_command_substitution((*dst), utils);
    del_extra_quotes((*dst));

    return 0;
}

// int mx_parse_line(t_cmd_utils *utils, char *line, char ***dst)
// {
//     char *mod_line = mx_strtrim(line);
//     if (check_odd_quotes(mod_line) != 0)
//         return 1;

//     (*dst) = NULL;

//     char *tmp = mod_line;
//     int space_index = 0;
//     int i;

//     for (i = 0; space_index != -1; i++)
//     {
//         space_index = mx_get_char_index(tmp, ' ');
//         int backslash_index = mx_get_substr_index(tmp, "\\ ");

//         while (space_index == backslash_index + 1 && backslash_index != -1)
//         {
//             tmp += space_index + 1;
//             space_index = mx_get_char_index(tmp, ' ');
//             backslash_index = mx_get_substr_index(tmp, "\\ ");
//         }

//         (*dst) = mx_realloc((*dst), (i + 2) * sizeof(char *));

//         if (space_index == -1)
//         {
//             (*dst)[i] = mx_strndup(mod_line, mx_strlen(mod_line));
//         }
//         else if (mod_line[0] == '"')
//         {
//             (*dst)[i] = mx_strndup(mod_line, mx_get_char_index(mod_line + 1, '\"') + 2);
//             tmp += mx_get_char_index(mod_line + 1, '\"') + 2;
//         }
//         else if (mod_line[0] == '\'')
//         {
//             (*dst)[i] = mx_strndup(mod_line, mx_get_char_index(mod_line + 1, '\'') + 2);
//             tmp += mx_get_char_index(mod_line + 1, '\'') + 2;
//         }
//         else if (mod_line[0] == '$' && mod_line[1] == '(')
//         {
//             int close_parenthesis_idx = get_close_parenthesis_idx(mod_line);
            
//             (*dst)[i] = mx_strndup(mod_line, close_parenthesis_idx + 1);
//             tmp += close_parenthesis_idx + 1;
//         }
//         else
//         {
//             tmp += space_index + 1;
//             (*dst)[i] = mx_strndup(mod_line, mx_strlen(mod_line) - mx_strlen(tmp) - 1);
//         }

//         while (mx_isspace(tmp[0]))
//         {
//             tmp++;
//         }

//         mod_line = tmp;
//     }

//     (*dst)[i] = NULL;

//     handle_backslashes((*dst));
//     mx_param_expansion((*dst));  

//     if (mx_tilde_expansion((*dst)) != 0)
//         return 1;

//     mx_command_substitution((*dst), utils);
//     del_extra_quotes((*dst));

//     return 0;
// }
