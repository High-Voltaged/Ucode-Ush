#include "../inc/ush.h"

void parse_line(t_cmd_utils *utils, char *line)
{
    char *tmp = line;
    
    // int num_back_slash = mx_count_substr(line, "\\ ");


    //for line with func name without args
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

    // utils->cmd = mx_strndup(line, mx_get_char_index(tmp, ' '));
    // line += mx_get_char_index(tmp, ' ') + 1;
        tmp += mx_get_char_index(tmp, ' ') + 1;
    utils->cmd = mx_strndup(line, mx_get_substr_index(line, tmp));
    line += mx_get_substr_index(line, tmp) + 1;
    // tmp = line;
    
    int args_len = mx_count_words(line, ' ') - mx_count_substr(line, "\\ ");
    utils->args = malloc((args_len) * sizeof(char*));

    int i;
    for (i = 0; i < args_len; i++)
    {
        space_index = mx_get_char_index(tmp, ' ');
        back_slash_index = mx_get_substr_index(tmp, "\\ ");

        while (space_index == back_slash_index + 1)
        {
            tmp += mx_get_char_index(tmp, ' ') + 1;

            space_index = mx_get_char_index(tmp, ' ');
            back_slash_index = mx_get_substr_index(tmp, "\\ ");
        }
        

        // if (num_back_slash && back_slash_index != -1)
        // {
        //     if (space_index == back_slash_index + 1)
        //     {
        //         tmp += mx_get_char_index(tmp, ' ') + 1;
        //     }
        //     else 
        //     {
        //         utils->args[i] = mx_strndup(line, mx_get_char_index(line, ' '));
        //         line += mx_get_char_index(line, ' ') + 1;
        //     }
        // }
        // else
        // {

            // utils->args[i] = mx_strndup(line, mx_get_char_index(tmp, ' '));
            // line += mx_get_char_index(tmp, ' ') + 1;
            utils->args[i] = mx_strndup(line, mx_get_substr_index(line, tmp));
            line += mx_get_substr_index(line, tmp) + 1;

    // tmp = line;

        // }
                
    }
    utils->args[i] = NULL;

}

int main() {

    int status = 0;
    t_cmd_utils* utils = malloc(sizeof(*utils));
    mx_ush_init(&utils);


    while (1) {

        printf("u$h> ");
        char* line = mx_read_line();
        // utils->cmd = mx_strdup(line);

        parse_line(utils, line);

        printf("name: %s; ", utils->cmd);
        for (int i = 0; utils->args[i] != NULL; i++)
        {
            printf("%s, ", utils->args[i]);
        }
        printf("\n");
        
        mx_strdel(&line);

        // status = mx_exec(utils);

        mx_strdel(&utils->cmd);

    }

    return status; 

}
