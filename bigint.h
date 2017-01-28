#ifndef _BIGINT_H_
#define _BIGINT_H_
#include <iostream>

typedef unsigned long long int Unit;

class BigInt {
    Unit big, little;
public:

    /* Basic constructors */
    BigInt();
    BigInt(Unit);
    BigInt(Unit big, Unit little);
    BigInt(const BigInt& a);

    /* This will be used only for multiplication by 3 */
    BigInt& operator*=(Unit);

    /* This will be used only for dividing by 2 */
    BigInt& operator>>=(Unit);

    BigInt& operator++();

    /* Used for calculating next assignment */
    BigInt& operator+=(const BigInt&);

    /* Used for getting parity */
    Unit operator&(Unit);

    bool operator>=(const BigInt&);
    bool operator==(const BigInt&);
    bool operator<=(const BigInt&);
    bool operator!=(const BigInt&);
    bool operator>(const BigInt&);
    bool operator<(const BigInt&);
};

// fair estimate, around 2^128/3-1
const BigInt LARGEST_MANAGEABLE_NUMBER(UINT64_MAX/3-1, 0);

std::ostream& operator<<(std::ostream& iostream, const BigInt&);

#endif
