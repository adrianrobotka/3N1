#ifndef COLLATZ_TESTER_METHODS_H
#define COLLATZ_TESTER_METHODS_H

#include <string>
#include <mutex>
#include <sstream>
#include <iostream>
#include <thread>

using namespace std;

/*
 * --------------------------------------------------- Constants -----------------------------------------------------
 */

// Our big number type
typedef unsigned long long int BigInt;

/*
 * --------------------------------------------------- Global vars ---------------------------------------------------
 */

// Mutex for the log
mutex log_mutex;

// Mutex for the assignment process
mutex assign_mutex;

// A thread will enumerate ASSIGNMENT_STEP numbers. This should be timed to 10 sec.
// TODO check: "This should be timed to 10 sec."
BigInt assignment_step = 1000000;

// The next assignment number
BigInt next_assignment_start = 1;

/*
 * ----------------------------------------------------- Methods -----------------------------------------------------
 */

// Get number of supported threads
int getSupportedThreads();

// Cast int to string
string strCast(int);

// Log to console
void log(string message);

// Returns an assignment for a thread
void getNextAssignment(BigInt *start, BigInt *end);

// Thread worker. This should never ends.
void launchThread();

// Do Collatz algorithm on the number
inline void enumerateNumber(BigInt numberToTest);

#endif //COLLATZ_TESTER_METHODS_H
