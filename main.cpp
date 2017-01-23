#include <iostream>

using namespace std;

/**
 * Do Collatz algorithm on the number
 *
 * @param numberToTest Number to enumerate
 */
inline void enumerateNumber(unsigned long long int numberToTest) {
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
 * Entry point
 *
 * @return 0 or noting, in better case ;)
 */
int main() {
    ios::sync_with_stdio(false);

    unsigned long long int numberToTest = 2;
    unsigned short int index = 1;

    // until overflow
    while (numberToTest != 1) {
        if (index == 1) {
            cout << ".";
            cout.flush();
        }

        enumerateNumber(numberToTest);

        numberToTest++;
        index++;
    }

    return 0;
}