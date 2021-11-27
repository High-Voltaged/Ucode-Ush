#include "../inc/ush.h"

static void print_err_char(char flag) {
    
    write(2, &flag, 1);

}

void mx_print_odd_quotes_err() {

    mx_printerr("Odd number of quotes.\n");
    
}

void mx_print_too_many_args_err(const char* cmd) {

    mx_printerr(cmd);
    mx_printerr(": ");
    mx_printerr("too many arguments\n");
    
}

void mx_print_cmd_err(const char* cmd, const char* error) {

    mx_printerr(cmd);
    mx_printerr(": ");
    mx_printerr(error);
    mx_printerr("\n");

}

void mx_print_flag_err(const char* cmd, char flag) {

    // mx_printerr(USH_STR);
    mx_printerr(cmd);
    mx_printerr(": bad option: -");
    print_err_char(flag);
    mx_printerr("\n");

}

void mx_print_env_option_err(char flag, const char* cmd) {

    // mx_printerr(USH_STR);
    mx_printerr(cmd);
    mx_printerr(": invalid option -- ");
    print_err_char(flag);
    mx_printerr("\n");
    mx_printerr(ENV_USAGE);

}

void mx_print_env_arg_err(char flag) {

    mx_printerr("env: option requires an argument -- ");
    print_err_char(flag);
    mx_printerr("\n");
    mx_printerr(ENV_USAGE);

}

void mx_print_env_error(const char* error, const char* env_util) {

    mx_printerr("env: ");
    mx_printerr(env_util);
    mx_printerr(": ");
    mx_printerr(error);
    mx_printerr("\n");

}
