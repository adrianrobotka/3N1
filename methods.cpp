#include "methods.h"

/**
 * Get number of supported threads by the program. It's equals to the number of cores or threads in the CPU
 * @return number of supported threads
 */
// Split declaration from initialisation: supportedThreads not const enough
// according to g++ (idk why it doesn't work)
// TODO check comment ahead
int getSupportedThreads() {
    return thread::hardware_concurrency();
}

/**
 * Cast int to string
 * @return string from int
 */
string strCast(int n) {
    // Convert to number
    stringstream ss;
    ss << n;
    return ss.str();
}

/**
 * Cast BigInt to string
 * @return string from int
 */
string strCast(BigInt n) {
    // Convert to number
    stringstream ss;
    ss << n;
    return ss.str();
}

/*
 * Log to console
 */
void log(string message) {
    // Declare the output
    string prompt;
    // Get the time
    // source: http://stackoverflow.com/questions/16357999/current-date-and-time-as-string
    time_t rawTime;
    struct tm *timeInfo;
    char datetime[80];
    time(&rawTime);
    timeInfo = localtime(&rawTime);
    // Set the time string format
    strftime(datetime, 80, "%Y-%m-%d %I:%M:%S", timeInfo);
    // Add prompt sign to the output
    prompt += datetime;

    // Thread lock
    lock_guard<mutex> lock(log_mutex);

    cout << prompt << message << endl;
}

/**
 * Returns the next assignment for a thread
 */
void getNextAssignment(BigInt *start, BigInt *end) {
    // Thread lock
    lock_guard<mutex> lock(assign_mutex);

    *start = next_assignment_start;

    // Increase global assignment
    next_assignment_start += assignment_step;

    *end = next_assignment_start;
}

/**
 * A thread worker. This should never ends.
 */
void launchThread() {
    BigInt start, end;

    // Yupp, yupp
    while (true) {
        // Get next range for the number enumerations
        getNextAssignment(&start, &end);

        // Enumerate numbers in the range
        for (BigInt i = start; i < end; i++) {
            enumerateNumber(i);
        }

        // Give some sign of life XD
        log("Assignment " + strCast(start) + " complete.");
    }
}

/**
 * Do Collatz algorithm on the number
 *
 * @param numberToTest Number to enumerate
 */
inline void enumerateNumber(BigInt numberToTest) {
    while (numberToTest != 1) {
        if (numberToTest % 2 == 0) {
            numberToTest = numberToTest >> 1;
        } else {
            numberToTest *= 3;
            numberToTest++;
        }
    }
}