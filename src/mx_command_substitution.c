#include "../inc/ush.h"

static void insert_arr_in_elem_of_arr(char ***arr, char **new_elems, int insert_idx)
{
    int arr_len = 0;
    int new_elems_len = 0;

    while (arr[arr_len] != NULL)
    {
        arr_len++;
    }
    while (new_elems[new_elems_len] != NULL)
    {
        new_elems_len++;
    }
    
    (*arr) = mx_realloc((*arr), sizeof(char*) * (arr_len + new_elems_len));

    mx_strdel(&(*arr)[insert_idx]);
    for (int i = insert_idx + 1; (*arr)[i] != NULL; i++)
    {
        (*arr)[i + new_elems_len] = mx_strdup((*arr)[i]);
        mx_strdel(&(*arr)[i]);
    }

    for (int i = insert_idx, j = 0; j < new_elems_len; i++, j++)
    {
        (*arr)[i] = mx_strdup(new_elems[j]);
    }
    // arr[arr_len - 1] = "sdfsdf";
    (*arr)[arr_len + new_elems_len - 1] = NULL;
    
}

void mx_command_substitution(char ***args, t_cmd_utils* utils)
{

    int dollar_pos;
    char *to_replace = NULL;
    char *cmd = NULL;

    for (int i = 0; (*args)[i] != NULL; i++)
    {
        if ((*args)[i][0] == '\'' || mx_strlen((*args)[i]) <= 1)
        {
            continue;
        }

        char *tmp = (*args)[i];
        // char **new_args = NULL;
        
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
                char *result = mx_strdup("");
                mx_parse_line(utils, cmd, &cmd_args);

                if (mx_strcmp(cmd_args[0], "") != 0) {
                    
                    result = mx_cmd_exec(utils, cmd_args);

                }

                if ((*args)[i][0] == '\"')
                {
                    result = mx_strtrim(result);
                }
                else
                {
                    result = mx_del_extra_spaces(result);
                }
                
                to_replace = mx_strndup(&tmp[dollar_pos], mx_strlen(cmd) + 3);

                (*args)[i] = mx_replace_substr_free((*args)[i], to_replace, result);
                
                // mx_del_strarr(&new_args);
                // new_args = mx_strsplit(result, ' ');

                mx_strdel(&result);
                mx_strdel(&cmd);
                tmp = (*args)[i];
            }
        }

                // printf("HERE\n");
        // if (new_args != NULL && (*args)[i][0] != '\"')
        // {
        //     insert_arr_in_elem_of_arr(args, new_args, i);
        // }

        // mx_print_strarr((*args), "--");

        // mx_del_strarr(&new_args);
    }
}
