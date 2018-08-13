/*
 * Implementation of cooperative multitasking using setjmp and longjmp
 *
 * Just wait for coroutines, really!
 */

#include <setjmp.h>
#include <stdio.h>

jmp_buf mainTask, childTask;

int child_finished = 0;

void child (void) {
    while (1) {
        printf("Child loop begin\n");
        if (!setjmp(childTask)) longjmp(mainTask, 1); // yield - invalidates childTask in C99

        printf("Child loop end\n");
        if (!setjmp(childTask)) longjmp(mainTask, 1); // yield - invalidates childTask in C99

        child_finished = 1; // signal main that child has finished
        longjmp(mainTask, 1);
    }
}

void call_with_cushion (void) {
    char space[1000]; // reserve enough space for main to run
    space[999] = 1; // do not optimize array out of existence
    child();
}

int main(void) {
    if (!setjmp(mainTask)) {
        call_with_cushion(); // child never returns - yield
    }
    // execution resumes after this "}" after first time that child yields

    while (1) {
        if (child_finished) {
            break;
        }
        printf("Parent\n");
        if (!setjmp(mainTask)) {
            longjmp(childTask, 1); // yield - note that this is undefined under C99
        }
    }
}
