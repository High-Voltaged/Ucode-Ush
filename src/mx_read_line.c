#include "../inc/ush.h"

char* mx_read_line() {

    char* line = NULL;
    size_t length = 0;

    if (getline(&line, &length, stdin) == -1) {

        free(line);
        perror("getline");
        exit(1);

    }
    return line;

}
