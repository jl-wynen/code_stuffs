#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void signal_callback_handler(int signum) {
    printf("Caught signal %d\n", signum);
    
    // exit(signum);
}

int main() {
    signal(SIGINT, signal_callback_handler);

    while(1) {
        printf("Programm processing stuff\n");
        sleep(1);
    }

    return EXIT_SUCCESS;
}
