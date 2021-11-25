#include "../inc/ush.h"

static void handle_backslashes(char **args)
{
    if (!mx_strcmp(args[0], "echo"))
    {
        return;
    }
    
    for (int i = 1; args[i] != NULL; i++)
    {
        args[i] = mx_replace_substr(args[i], "\\ ", " ");
        args[i] = mx_replace_substr(args[i], "\\`", "`");
        args[i] = mx_replace_substr(args[i], "\\'", "'");
        args[i] = mx_replace_substr(args[i], "\\\"", "\"");

        // mx_strcmp(args[0], "echo") != 0 ? args[i] = mx_replace_substr(args[i], "\\\\", "\\") : (void) 0;//?
    }
}

// "sdfsdfsd sdfsdfs" - del quotes and join, 'dsfs' - del quotes
// static void handle_quotes(char **args)
// {

//     for (int i = 1; args[i] != NULL; i++)
//     {
//         int q_index = mx_get_char_index(args[i], '\'');
        
//     }

// }

void mx_parse_line(t_cmd_utils *utils, char *line)
{
    char *mod_line = mx_del_extra_spaces(line);

    //for line with only func name without args
    if ((mx_count_words(mod_line, ' ') - mx_count_substr(mod_line, "\\ ")) <= 1)
    {
        utils->args = malloc(2 * sizeof(char*));
        utils->args[0] = mx_strndup(mod_line, mx_strlen(mod_line));
        utils->args[1] = NULL;
        return;
    }
    //

    int args_len = mx_count_words(mod_line, ' ') - mx_count_substr(mod_line, "\\ ");
    utils->args = malloc((args_len + 1) * sizeof(char*));

    char *tmp = mod_line;
    int i;

    for (i = 0; i < args_len - 1; i++)
    {
        int space_index = mx_get_char_index(tmp, ' ');
        int back_slash_index = mx_get_substr_index(tmp, "\\ ");

        while (space_index == back_slash_index + 1)
        {
            tmp += mx_get_char_index(tmp, ' ') + 1;

            space_index = mx_get_char_index(tmp, ' ');
            back_slash_index = mx_get_substr_index(tmp, "\\ ");
        }

        tmp += space_index + 1;
        utils->args[i] = mx_strndup(mod_line, mx_strlen(mod_line) - mx_strlen(tmp) - 1);
        mod_line = tmp;
    }

    //save last elem of mod_line without '\n'
    utils->args[i] = mx_strndup(mod_line, mx_strlen(mod_line));

    utils->args[args_len] = NULL;

    handle_backslashes(utils->args);
    
}