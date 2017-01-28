#include "methods.h"

/*
 * --------------------------------------------------- Global vars ---------------------------------------------------
 */

// Mutex for the log
mutex log_mutex;

// Mutex for the assignment process
mutex assign_mutex;

// A thread will enumerate ASSIGNMENT_STEP numbers. This is a MAGIC constant
constexpr int ASSIGNMENT_STEP = 10000;

// The next assignment number. This means already checked values too
BigInt next_assignment_start = 1;

/**
 * If any error happens, all the threads stop
 * Must be atomic! (this way we do not need mutexes)
 */
bool run_forrest_run = true;

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
void getNextAssignment(BigInt *start) {
    // Thread lock
    lock_guard<mutex> lock(assign_mutex);

    *start = next_assignment_start;

    // Increase global assignment
    next_assignment_start += ASSIGNMENT_STEP;
}

/**
 * Call when the an error occoure in the Collatz number enumarition process
 */
inline void setCollatzErrorFlag() {
    run_forrest_run = false;
}

/**
 * A thread worker. This should never ends.
 */
void launchThread() {
    BigInt start;

    uint16_t chunker = 1;

    // Yupp, yupp
    while (run_forrest_run) {
        // Get next range for the number enumerations
        getNextAssignment(&start);

        // Enumerate numbers in the range
        for (int i = 0; i < ASSIGNMENT_STEP; ++i) {
            if (enumerateNumber(i)) {
                /**
                 * stop on error
                 * only here can we know what failed
                 */

                ostringstream message;
                message << "FATAL ERROR OCCURRED!!!" << endl;
                message << "Assignment " << start << "-" << " failed!" << endl;
                message << "Testing number " << i << " failed! " << endl;
                message << "Thread failed, exiting!" << endl;
                log(message);

                // Stop all the threads
                setCollatzErrorFlag();
                return;
            }
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
 * @return True if the process fails
 */
bool enumerateNumber(BigInt numberToTest) {
    /**
    * Lower_boundary is 2, so it always stops if the numberToTest is 1
    */
    while (numberToTest >= next_assignment_start) {
        /**
         * Test if the number is even
         * Do *not* forget the parentheses!
         */
        if ((numberToTest & 1) == 0) {
            numberToTest >>= 1;
        } else {
            /**
             * This is where the number grows, so we test it only here
             */
            if (numberToTest > LARGEST_MANAGEABLE_NUMBER) {
                ostringstream message;
                message << "Integer overflow ERROR OCCURRED!!!" << endl;
                message << "Testing number " << numberToTest << " failed! " << endl;
                message << "" << endl;
                log(message);
                return 1;
            }
            numberToTest *= 3;
            ++numberToTest;
            /**
             * Minor optimisation: numberToTest is always even here and does
             * not change stopping condition
             */
            numberToTest >>= 1;
        }
    }

    /* No error */
    return 0;
}