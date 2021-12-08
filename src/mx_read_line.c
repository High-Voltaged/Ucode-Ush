#include "../inc/ush.h"

static int mx_getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

static void handle_arrows()
{
    mx_getch(); // skip the [
    switch (mx_getch())
    { // the real value
    case 'A':
        // code for arrow up
        break;
    case 'B':
        // code for arrow down
        break;
    case 'C':
        // code for arrow right
        break;
    case 'D':
        // code for arrow left
        break;
    }
}

static int handle_keycode(char **line, int *pos, char keycode)
{
    if (keycode == '\033') // if the first value is esc
    {
        handle_arrows();
    }
    else if (keycode == MX_ENTER)
    {
        mx_printchar('\n');
        return 1;
    }
    else if (keycode == MX_CTRL_C)
    {
        for (int i = 0; i < mx_strlen(*line); i++)
        {
            (*line)[i] = '\0';
        }
        mx_printchar('\n');
        return 1;
    }
    else if (keycode == MX_CTRL_D)
    {
        if ((*pos) == 0)
        {
            exit(EXIT_FAILURE);
        }
    }
    else if (keycode == MX_BACKSPACE)
    {
        if ((*pos) > 0)
        {
            mx_printchar('\b');
            mx_printchar(' ');
            mx_printstr("\033[1D");
            (*line)[--(*pos)] = '\0';
        }
    }
    else
    {
        mx_printchar(keycode);
        (*line)[(*pos)++] = keycode;
    }

    return 0;
}

char **mx_read_line()
{
    char *line = mx_strnew(1024);
    char keycode = 0;
    int pos = 0;

    while ((keycode = mx_getch()))
    {
        if (handle_keycode(&line, &pos, keycode))
        {
            break;
        }
    }

    char **separated = mx_strsplit(line, ';');
    mx_strdel(&line);

    return separated;
}
