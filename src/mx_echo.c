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

int octal_to_decimal(int n)
{
    int num = n;
    int dec_value = 0;
 
    // Initializing base value to 1, i.e 8^0
    int base = 1;
 
    int temp = num;
    while (temp) {
 
        // Extracting last digit
        int last_digit = temp % 10;
        temp = temp / 10;
 
        // Multiplying last digit with appropriate
        // base value and adding it to dec_value
        dec_value += last_digit * base;
 
        base = base * 8;
    }
 
    return dec_value;
}

void print_oct(char *str, int *letter_count)
{
    int oct_len = 0;
    while (mx_isdigit(str[oct_len]) && (oct_len <= 3) && (str[oct_len] != '\0')) // 3 is a max len of oct num in our shell
    {
        oct_len++;
        (*letter_count)++;
    }
    
    if (!oct_len)
    {
        return;
    }
    
    char *oct = mx_strndup(str, oct_len);

    int dec = octal_to_decimal(mx_atoi(oct));
    mx_printchar(dec);

    mx_strdel(&oct);
}

void print_hex(char *str)
{
    char *hex = mx_strndup(str, 2);
    int dec = mx_hex_to_nbr(hex);
    mx_printchar(dec);

    mx_strdel(&hex);
}

void print_bec(char *str, int *letter_count) {
        // wchar_t c = L'\U0001f602';
    if (str[2] == '\\' && str[3] != '\\' )
    {
        return;
    }
    else if (str[2] == '\\' && str[3] == '\\' )
    {
        mx_printchar('\\');
        (*letter_count) += 3;
        return;
    }
    
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

    case '0':
        if (mx_isdigit(str[3]))
        {
            print_oct(str + 3, letter_count);
            (*letter_count) += 2;
            break;   
        }
        else {
            mx_printchar('\\');
            (*letter_count) += 1;
            break;
        }

    case 'x':
        // mx_printchar('\x23');
        if (mx_isalpha(str[3]) || mx_isdigit(str[3]))
        {
            print_hex(str + 3);
            (*letter_count) += 4;//fix? is always hex number length is equal to 2?
            break;   
        }
        else {
            mx_printchar('\\');
            (*letter_count) += 1;
            break;
        }
    
    // case '\\':
    //     mx_printchar('\\');
    //     (*letter_count) += 2;
    //     break;
    
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
        // if (str[i] == '\\' && str[i + 1] != '\\')
        // {
        //     continue;
        // }
        // else 
        if (str[i] == '\\' && str[i + 1] == '\\')
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
