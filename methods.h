#ifndef COLLATZ_TESTER_METHODS_H
#define COLLATZ_TESTER_METHODS_H

#include <string>
#include <mutex>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <iostream>

using namespace std;

/*
 * --------------------------------------------------- Constants -----------------------------------------------------
 */

// Our big number type
typedef unsigned long long int BigInt;

/*
 * ------------------------------------------------- Method headers --------------------------------------------------
 */

// Get number of supported threads
int getSupportedThreads();

// Log to console
void log(ostringstream &message);

// Returns an assignment for a thread
void getNextAssignment(BigInt *start, BigInt *end);

// Thread worker. This should never ends.
void launchThread();

// Do Collatz algorithm on the number
inline void enumerateNumber(BigInt numberToTest);

#endif //COLLATZ_TESTER_METHODS_H
