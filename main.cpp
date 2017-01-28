#include <thread>
#include <mutex>
#include <sstream>
#include "methods.h"

using namespace std;

// Worker threads used in the program
thread *threads;

/**
 * Entry point of the program, you know the start point of the bug hunting :D
 *
 * @return 0 or noting, in better case ;)
 */
int main() {
    // Optimize input, output performance
    ios::sync_with_stdio(false);

    // Get number of working threads
    int supportedThreads = getSupportedThreads();
    ostringstream message;
    message << "Supported threads: " << supportedThreads;
    log(message);

    // Create working threads
    threads = new thread[supportedThreads];
    for (int i = 0; i < supportedThreads; i++) {
        // launch thread
        threads[i] = thread(launchThread);
    }

    /*
     * There is some work in the background...
     * Because it's a multi-thread program :)
     */

    // waiting for end of the threads
    // This point should never reached
    for (int i = 0; i < supportedThreads; i++) {
        threads[i].join();
        message.clear();
        message << "Thread # " << i << " joined.";
        log(message);
    }

    message.clear();
    message << "End of the program";
    log(message);

    delete[] threads;

    return EXIT_FAILURE;
}
