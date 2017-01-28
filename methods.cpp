#include "methods.h"

/*
 * --------------------------------------------------- Global vars ---------------------------------------------------
 */

// Mutex for the log
mutex log_mutex;

// Mutex for the assignment process
mutex assign_mutex;

// A thread will enumerate ASSIGNMENT_STEP numbers. This is a MAGIC constant
constexpr BigInt assignment_step = 100000;

// The next assignment number
BigInt next_assignment_start = 1;

/*
 * ------------------------------------------------- Method bodies --------------------------------------------------
 */

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

/*
 * Log to console
 */
void log(ostringstream &message) {
    /*
     * Get time to the log prefix
     */
    using namespace std::chrono;
    system_clock::time_point now = system_clock::now();
    system_clock::duration tp = now.time_since_epoch();
    time_t tt = system_clock::to_time_t(now);
    tm t = *localtime(&tt);

    // Buffer to write log prefix
    char buffer[50];
    sprintf(buffer, "[%04u-%02u-%02u %02u:%02u:%02u.%03u] ", t.tm_year + 1900,
            t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
            static_cast<unsigned>(tp / milliseconds(1)));

    // Thread lock
    lock_guard<mutex> lock(log_mutex);

    cout << buffer << message.str() << endl;
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

    uint8_t chunker = 0;

    // Yupp, yupp
    while (true) {
        // Get next range for the number enumerations
        getNextAssignment(&start, &end);

        // Enumerate numbers in the range
        for (BigInt i = start; i < end; i++) {
            enumerateNumber(i);
        }

        if (chunker == 0) {
            // Give some sign of life XD
            ostringstream message;
            message << "Assignment " << start << " complete.";
            log(message);
        }

        chunker++;
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