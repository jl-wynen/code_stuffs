#include <unistd.h>
#include <stdio.h>

int main() {

    if (isatty(STDOUT_FILENO))
        printf("To terminal\n");
    else
        printf("Not to terminal\n");

    return 0;
}
