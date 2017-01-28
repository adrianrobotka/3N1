#include <iostream>
#include <thread>
#include <mutex>

// TODO: test if a thread has an infinite cycle

constexpr int ASSIGNMENT_SCALE = 1000000;

using namespace std;

/**
 * Our big number type
 */
typedef unsigned long long int BigInt;

/**
 * Split declaration from initialisation: supportedThreads not const enough
 * according to g++ (idk why it doesn't work)
 */
const unsigned supportedThreads = thread::hardware_concurrency();
thread *threads;

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
inline void enumerateNumber(BigInt numberToTest) {
    /**
     * Lower_boundary is 2, so it always stops if the numberToTest is 1
     */
    while (numberToTest >= lower_boundary) {
        if (numberToTest % 2 == 0) {
            numberToTest = numberToTest >> 1;
        } else {
            numberToTest *= 3;
            numberToTest++;
            /**
             * Minor optimisation: numberToTest is always even here and does
             * not change stopping condition
             */
            numberToTest = numberToTest >> 1;
        }
    }
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
    while (true) {
        getAssignment(&start, &end);
        for (BigInt i = start; i < end; i++) {
            enumerateNumber(i);
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
