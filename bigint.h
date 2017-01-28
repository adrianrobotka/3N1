#ifndef _BIGINT_H_
#define _BIGINT_H_
#include <iostream>

/* Two units will make up BigInt */
typedef unsigned long long int Unit;
constexpr Unit UNIT_NUMBER_OF_BITS = 64;

/* See divide by 2 for usage */
constexpr Unit UNIT_LARGEST_BIT = (Unit)2<<(UNIT_NUMBER_OF_BITS-1);

/* See multiplication by 3 for usage */
/* (-2^UNIT_NUMBER_OF_BITS) % 3 */
constexpr Unit MINUS_UNIT_MOD_3 = 2;
/* (-2*2^UNIT_NUMBER_OF_BITS) % 3 */
constexpr Unit MINUS_2_UNIT_MOD_3 = 1;

class BigInt {
    /* high is the high UNIT_NUMBER_OF_BITS bits
     * low is the low UNIT_NUMBER_OF_BITS bits
     * Basically the number will be high*2^UNIT_NUMBER_OF_BITS + low
     */
    Unit high, low;
public:

    /* Basic constructors */
    BigInt();
    BigInt(Unit);
    BigInt(Unit high, Unit low);
    BigInt(const BigInt& a);

    /* This will be used only for multiplication by 3 */
    BigInt& operator*=(Unit);

    /* This will be used only for dividing by 2 */
    BigInt& operator>>=(Unit);

    /* Increments by 1 */
    BigInt& operator++();

    /* Used for calculating next assignment */
    BigInt& operator+=(const BigInt&);

    /* Used for getting parity */
    Unit operator&(Unit);

    /* Comparisons */
    bool operator>=(const BigInt&);
    bool operator==(const BigInt&);
    bool operator<=(const BigInt&);
    bool operator!=(const BigInt&);
    bool operator>(const BigInt&);
    bool operator<(const BigInt&);

    friend std::ostream& operator<<(std::ostream& iostream, const BigInt&);
};

/**
 * The (number's maximum value)/3-1 is always manageable
 * This is because we cannot test overflow (without inline asm)
 * fair estimate, less than 2^128/3-1
 */
const BigInt LARGEST_MANAGEABLE_NUMBER(UINT64_MAX/3-1, 0);

std::ostream& operator<<(std::ostream& iostream, const BigInt&);

#endif
