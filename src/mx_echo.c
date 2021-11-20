#include "../inc/ush.h"

void mx_echo_add_flag(t_echo_flags** flags, char flag) {

    switch (flag) { 

    case 'n':
        (*flags)->n = 1;
        break;

    case 'e':
        (*flags)->e = 1;
        (*flags)->E = 0;
        break;

    case 'E':
        (*flags)->e = 0;
        (*flags)->E = 1;
        break;
    
    default: break;
    
    }

}

int mx_echo(t_cmd_utils* utils)
{
    int flag_count = 0;
    t_echo_flags* flags = malloc(sizeof(*flags));
    mx_echo_parse_flags(&flags, utils, &flag_count);

    for (int i = 1 + flag_count; utils->args[i]; i++)
    {
        mx_printstr(utils->args[i]);
        utils->args[i + 1] ? mx_printstr(" ") : (void) 0;
    }

    flags->n ? (void) 0 : mx_printstr("\n");
    

    return 0;
}
