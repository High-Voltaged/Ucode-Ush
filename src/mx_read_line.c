#include "../inc/ush.h"

char** mx_read_line(/*t_cmd_utils* utils*/) {

    char* line = NULL;
    size_t length = 0;

    if (getline(&line, &length, stdin) == -1) {

        free(line);
        exit(1);

    }

    // char s = 0;
    // char buffer[BUFFER_LEN];
    // int i = 0;
    // FILE* fd = fopen("log.txt", "a");
    // while ((s = getc(stdin))) {

    //     if (s == 10)
    //         break;

    //     if (s == EOF) {
    //         s = 0;
    //         fprintf(fd, "eof entered\n");
    //         if (i == 0) {
    //             fprintf(fd, "Exit now...\n");
    //             exit(EXIT_FAILURE);
    //         }
    //         continue;
    //     }
    //     buffer[i++] = s;

    // }
    // fclose(fd);
    // buffer[i] = '\0';
    char** separated = mx_strsplit(line, ';');
    mx_strdel(&line);
    return separated;

}
