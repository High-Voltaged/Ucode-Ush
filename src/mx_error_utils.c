#include "../inc/ush.h"

static void print_err_char(char flag) {
    
    write(2, &flag, 1);

}

void mx_print_pwd_flag_err(char flag) {

    mx_printerr("pwd: bad option: -");
    print_err_char(flag);
    mx_printerr("\n");

}

void mx_print_flag_err(char flag, const char* cmd) {

    mx_printerr(USH_STR);
    mx_printerr(cmd);
    mx_printerr(": invalid option -- ");
    print_err_char(flag);
    mx_printerr("\n");
    // mx_printerr();

}

void mx_print_env_error(char flag) {

    mx_printerr("env: option requires an argument -- '");
    print_err_char(flag);
    mx_printerr("'\n");

}
