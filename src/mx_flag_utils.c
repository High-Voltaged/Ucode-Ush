#include "../inc/ush.h"

bool mx_is_flag_found(char* flags, char flag) {

    for (size_t i = 0; i < sizeof(flags) / sizeof(flags[0]); ++i) {

        if (flag == flags[i])
            return true;

    }
    return false;

}


