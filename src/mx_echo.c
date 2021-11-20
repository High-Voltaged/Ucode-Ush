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

void print_bec(char *str, int *letter_count) {
        // wchar_t c = L'\U0001f602';

    switch (str[2])
    {
    case 'a':
        mx_printchar('\a');
        (*letter_count) += 2;
        break;
    
    case 'b':
        mx_printchar('\b');
        (*letter_count) += 2;
        break;
    
    case 'c':
        (*letter_count) += mx_strlen(str);
        break;

    case 'e':
        // mx_printchar('\e');
        mx_printchar(27);
        (*letter_count) += 2;
        break;

    case 'f':
        mx_printchar('\f');
        (*letter_count) += 2;
        break;
    
    case 'n':
        // printf("%lc", c);
        mx_printchar('\n');
        (*letter_count) += 2;
        break;
    
    case 't':
        mx_printchar('\t');
        (*letter_count) += 2;
        break;

    case 'v':
        mx_printchar('\v');
        (*letter_count) += 2;
        break;
    
    default:
        mx_printchar('\\');
        (*letter_count) += 1;
        break;
    }

}

void out_echo_e(char *str)
{
    for (int i = 0; i < mx_strlen(str); i++)
    {
        if (str[i] == '\\' && str[i + 1] != '\\')
        {
            continue;
        }
        else if (str[i] == '\\' && str[i + 1] == '\\')
        {
            print_bec(str + i, &i);
        }
        else
        {
            mx_printchar(str[i]);
        }
    }
    
}

int mx_echo(t_cmd_utils* utils)
{
    int flag_count = 0;
    t_echo_flags* flags = malloc(sizeof(*flags));
    mx_echo_parse_flags(&flags, utils, &flag_count);

    for (int i = 1 + flag_count; utils->args[i]; i++)
    {
        flags->e ? out_echo_e(utils->args[i]) : mx_printstr(utils->args[i]);
        utils->args[i + 1] ? mx_printstr(" ") : (void) 0;
    }

    flags->n ? (void) 0 : mx_printstr("\n");

    return 0;
}
