#include <iostream>
#include <thread>
#include <mutex>
#include <cstdint>

// TODO: test if a thread has an infinite cycle

using namespace std;

/**
 * Our big number type
 */
typedef unsigned long long int BigInt;

/**
 * The (number's maximum value)/3-1 is always manageable
 * This is because we cannot test overflow (without inline asm)
 */
const BigInt LARGEST_MANAGEABLE_NUMBER = UINT64_MAX/3-1;

const BigInt ASSIGNMENT_SCALE = 10000000;

/**
 * Split declaration from initialisation: supportedThreads not const enough
 * according to g++ (idk why it doesn't work)
 */
const unsigned supportedThreads = thread::hardware_concurrency();
thread *threads;

/**
 * If any error happens, all the threads stop
 * Must be atomic! (this way we do not need mutexes)
 */
int collatz_error = 0;
inline void setCollatzErrorFlag() {
    collatz_error = 1;
}

inline bool isCollatzErrorFlagSet() {
    return collatz_error;
}

/**
 * This mutex ensures that the writes on stdout don't get mixed
 */
std::mutex output_mutex;

/**
 * This mutex ensures that assign functions are not called simultaneously
 */
std::mutex assign_mutex;

/**
 *  Stores what to assign next
 */
BigInt next_assignment_start = 0;
BigInt next_assignment_end;

/**
 * Already checked values
 * initially 2 for minor optimisation (see enumerateNumber)
 */
BigInt lower_boundary = 2;


/**
 * Do Collatz algorithm on the number
 *
 * @param numberToTest Number to enumerate
 */
inline int enumerateNumber(BigInt numberToTest) {
    /**
     * Lower_boundary is 2, so it always stops if the numberToTest is 1
     */
    while (numberToTest >= lower_boundary) {
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
                std::lock_guard<std::mutex> lock(output_mutex);
                cout << "Integer overflow" << endl;
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

/**
 * Returns an assignment for a thread
 * The pointers must be initialized. The values pointed will be filled.
 * The assignment means that x must be tested if start <= x < end
 */
void getAssignment(BigInt *start, BigInt *end) {
    std::lock_guard<std::mutex> lock(assign_mutex);
    lower_boundary = *end;

    if (next_assignment_start == -1) {
        next_assignment_start = 2;
        next_assignment_end = ASSIGNMENT_SCALE;
    } else {
        next_assignment_start = next_assignment_end;
        next_assignment_end += ASSIGNMENT_SCALE;
    }
    *start = next_assignment_start;
    *end = next_assignment_end;
}

/**
 * A thread worker. This should never ends.
 */
void launchThread() {
    BigInt start, end;
    /* Stop if an error happens in a thread */
    while (!isCollatzErrorFlagSet()) {
        getAssignment(&start, &end);
        for (BigInt i = start; i < end; ++i) {
            if (enumerateNumber(i)) {
                /**
                 * stop on error
                 * only here can we know what failed
                 */
                std::lock_guard<std::mutex> lock(output_mutex);
                cout << "Assignment " << start << "-" << end << " failed!"
                     << endl;
                cout << "Testing number " << i << " failed! " << endl;
                cout << "Thread failed, exiting!" << endl;
                setCollatzErrorFlag();
                return;
            }
        }
        std::lock_guard<std::mutex> lock(output_mutex);
        cout << "Assignment " << start << "-" << end << " complete!" << endl;
    }
}

/**
 * Entry point
 *
 * @return 0 or noting, in better case ;)
 */
int main() {
    ios::sync_with_stdio(false);

    cout << "Supported threads: " << supportedThreads << endl;
    cout << "Start program" << endl;

    threads = new thread[supportedThreads];
    for (int i = 0; i < supportedThreads; i++) {
        threads[i] = thread(launchThread);
    }

    for (int i = 0; i < supportedThreads; i++) {
        threads[i].join();
        cerr << "Thread #" << i << " joined." << endl;
    }

    cerr << "END" << endl;

    delete[] threads;

    return 0;
}
