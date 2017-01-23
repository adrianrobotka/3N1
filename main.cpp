#include <iostream>
#include <thread>

using namespace std;

// Our big number type
typedef unsigned long long int BigInt;

const unsigned supportedThreads = thread::hardware_concurrency();
thread threads[supportedThreads];

BigInt boundary = 2;
const BigInt step = 1000000000;

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

/**
 * A thread worker. This should never ends.
 */
void launchThread() {

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

    for (int i = 0; i < supportedThreads; i++) {
        threads[i] = thread(launchThread);
    }

    for (int i = 0; i < supportedThreads; i++) {
        threads[i].join();
        cerr << "Thread #" << i << " joined." << endl;
    }

    cerr << "END" << endl;

    return 0;
}