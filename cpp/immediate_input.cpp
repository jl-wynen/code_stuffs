// only works for UNIX (-like)
// on windows: use conio.h and getch

#include <cstdio>
#include <termios.h>
#include <unistd.h>

int main() {
    // those store terminal settings
    termios oldt, newt;

    // get the original settings
    tcgetattr(STDIN_FILENO, &oldt);

    // copy them
    newt = oldt;

    // disable canonical mode -> do not buffer whole lines but send
    // input immediately
    newt.c_lflag &= ~static_cast<unsigned int>(ICANON);

    // also suppress linking input to output -> no longer display input automatically
    // newt.c_lflag &= ~static_cast<unsigned int>(ICANON|ECHO);

    // set new attributes now
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // test it
    printf("Enter something: ");
    int c = getchar();
    printf("\nYou entered: %c\n", c);

    // restore old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
