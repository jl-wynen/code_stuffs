#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "signalHandler.hpp"

using namespace std;

int main () {
    try {
        SignalHandler signalHandler;

        // Register signal handler to handle kill signal
        signalHandler.setupSignalHandlers();

        while (!signalHandler.gotExitSignal()) {
            sleep(1);
            cout << "hi there!" << endl;
        }

        return EXIT_SUCCESS;
    }
    catch (SignalException &e) {
        std::cerr << "SignalException: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
