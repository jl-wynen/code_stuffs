#include <signal.h>
#include <errno.h>

#include "signalHandler.hpp"

bool SignalHandler::mbGotExitSignal = false;

/**
 * Default Constructor
 */
SignalHandler::SignalHandler() {}

/**
 * Destructor
 */
SignalHandler::~SignalHandler() {}

/**
 * Returns the bool flag indication whether we recieved an exit signal
 * @return Flag indicating shutdown of program
 */
bool SignalHandler::gotExitSignal() {
    return mbGotExitSignal;
}

/**
 * Sets the bool flag indicating whether we recieved an exit signal
 */
void SignalHandler::setExitSignal(bool _bExitSignal) {
    mbGotExitSignal = _bExitSignal;
}

/**
 * Set exit signal to true.
 * @param[in] _ignored Not used but required by function prototype
 *                     to match required handler.
 */
void SignalHandler::exitSignalHandler [[noreturn]] (int _ignored) {
    mbGotExitSignal = true;
    throw SignalException("Stop right there!");
}

/**
 * Set up the signal handlers for CTRL-C
 */
void SignalHandler::setupSignalHandlers() {
    if (signal((int) SIGINT, SignalHandler::exitSignalHandler) == SIG_ERR) {
        throw SignalException("!!!! Error setting up signal handlers !!!!");
    }
}
