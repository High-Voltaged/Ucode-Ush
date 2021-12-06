#include "../inc/ush.h"

int mx_getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

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

        // if (s == 127) {
        //     mx_printchar('\b');
        //     mx_printchar(' ');
        // }

    //     if (s == EOF) {
    //         s = 0;
    //         fprintf(fd, "eof entered\n");
    //         if (i == 0) {
    //             fprintf(fd, "Exit now...\n");
    //             exit(EXIT_FAILURE);
    //         }
    //         // mx_getch();
    //     }
    //     mx_printchar(s);
    //     buffer[i++] = s;

    // }
    // mx_printchar('\n');
    // fclose(fd);
    // buffer[i] = '\0';
    char** separated = mx_strsplit(line, ';');
    mx_strdel(&line);
    return separated;

}
