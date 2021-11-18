#include "../inc/ush.h"

void mx_parse_line(t_cmd_utils *utils, char *line)
{
    char *tmp = line;

    //for line with only func name without args
    if ((mx_count_words(line, ' ') - mx_count_substr(line, "\\ ")) == 1)
    {
        utils->cmd = mx_strndup(line, mx_strlen(line) - 1);
        return;
    }

    int space_index = mx_get_char_index(tmp, ' ');
    int back_slash_index = mx_get_substr_index(tmp, "\\ ");

    while (space_index == back_slash_index + 1)
    {
        tmp += mx_get_char_index(tmp, ' ') + 1;

        space_index = mx_get_char_index(tmp, ' ');
        back_slash_index = mx_get_substr_index(tmp, "\\ ");
    }
    tmp += mx_get_char_index(tmp, ' ') + 1;
    utils->cmd = mx_strndup(line, mx_get_substr_index(line, tmp) - 1);
    line += mx_get_substr_index(line, tmp);
    
    int args_len = mx_count_words(line, ' ') - mx_count_substr(line, "\\ ");
    utils->args = malloc((args_len) * sizeof(char*));

    int i;
    for (i = 0; i < args_len - 1; i++)
    {
        space_index = mx_get_char_index(tmp, ' ');
        back_slash_index = mx_get_substr_index(tmp, "\\ ");

        while (space_index == back_slash_index + 1)
        {
            tmp += mx_get_char_index(tmp, ' ') + 1;

            space_index = mx_get_char_index(tmp, ' ');
            back_slash_index = mx_get_substr_index(tmp, "\\ ");
        }

        tmp += mx_get_char_index(tmp, ' ') + 1;
        utils->args[i] = mx_strndup(line, mx_get_substr_index(line, tmp) - 1);
        line += mx_get_substr_index(line, tmp);         
    }
    
    //save last elem of line without '\n'
    utils->args[i++] = mx_strndup(line, mx_strlen(line) - 1);

    utils->args[i] = NULL;
}
