#include <thread>
#include "methods.h"

using namespace std;

thread *threads;

/**
 * Entry point
 *
 * @return 0 or noting, in better case ;)
 */
int main() {
    // Optimize input, output performance
    ios::sync_with_stdio(false);

    // Get number of working threads
    int supportedThreads = getSupportedThreads();
    log("Supported threads: " + strCast(supportedThreads));

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
        log("Thread # " + strCast(i) + " joined.");
    }

    log("End of the program");

    delete[] threads;

    return 0;
}
