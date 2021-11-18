#include "../inc/ush.h"

bool mx_is_flag_found(char* flags, char flag) {

    for (int i = 0; i < mx_strlen(flags); ++i) {

        if (flag == flags[i])
            return true;

    }
    return false;

}


