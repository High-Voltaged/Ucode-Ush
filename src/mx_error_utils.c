#include "../inc/ush.h"

static void print_err_char(char flag) {
    
    write(2, &flag, 1);

}

void mx_print_flag_err(char flag) {

    mx_printerr(USH_STR);
    mx_printerr("invalid option -- ");
    print_err_char(flag);
    mx_printerr("\n");
    // mx_printerr();

}
