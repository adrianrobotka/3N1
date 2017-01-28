#include "bigint.h"
#include <cassert>

BigInt::BigInt() : high(0), low(0) {}

BigInt::BigInt(Unit low) : high(0), low(low) {}

BigInt::BigInt(Unit high, Unit low) : high(high), low(low) {}

BigInt::BigInt(const BigInt &a) : high(a.high), low(a.low) {}

/* This will be used only for multiplication by 3 */
BigInt &BigInt::operator*=(Unit x) {
    assert(x == 3);
    /**
     * The math:
     * x *= 3*x;
     * x <- 3*x mod (2^64)
     * There are 3 cases: Where the new value will be ...
     *  1) 3*x
     *  2) 3*x-2^64 or
     *  3) 3*x-2^65
     * These can be differentiated by the new value mod 3.
     * Thus we can conclude what was the lost data
     */
    high *= 3;
    assert(high % 3 == 0); /* Assert no overflow happened in high */
    low *= 3;
    /* In the first case there is no carry */
    /* 2nd case: 1 carry */
    if (low % 3 == MINUS_UNIT_MOD_3)
        ++high;
    /* 3rd case: 2 carry */
    if (low % 3 == MINUS_2_UNIT_MOD_3)
        high += 2;
    return *this;
}

/* This will be used only for dividing by 2 */
BigInt &BigInt::operator>>=(Unit x) {
    assert(x == 1);
    low >>= 1;
    /* handle carry: high's lowest bit will be transferred to low's highest */
    if ((high & 1) == 1)
        low |= UNIT_LARGEST_BIT;
    high >>= 1;
    return *this;
}

BigInt &BigInt::operator++() {
    ++low;
    /* handle carry: if low is 0, then an overflow happened
     * in decimal system: 09+1 == 10
     */
    if (low == 0) {
        ++high;
        assert(high != 0); /* Assert no overflow happened in high */
    }
    return *this;
}

/* Used for calculating next assignment */
BigInt &BigInt::operator+=(const BigInt &x) {
    low += x.low;
    /* On overflow, increment high */
    if (low < x.low) {
        ++high;
        assert(high != 0); /* Assert no overflow happened in high */
    }
    high += x.high;
    assert(high >= x.high); /* Assert no overflow happened in high */
    return *this;
}

/* Used for getting parity */
Unit BigInt::operator&(Unit x) {
    return low & x;
}

bool BigInt::operator>=(const BigInt &x) {
    /* If the high-s are different, then we compare them */
    if (high != x.high) return high >= x.high;
    /* Otherwise the low-s will define the result */
    return low >= x.low;
}

bool BigInt::operator==(const BigInt &x){
    if (high != x.high) return high == x.high;
    return low == x.low;
}

bool BigInt::operator<=(const BigInt &x) {
    if (high != x.high) return high <= x.high;
    return low <= x.low;
}

bool BigInt::operator!=(const BigInt &x) {
    if (high != x.high) return high != x.high;
    return low != x.low;
}

bool BigInt::operator>(const BigInt &x) {
    if (high != x.high) return high > x.high;
    return low > x.low;
}

bool BigInt::operator<(const BigInt &x) {
    if (high != x.high) return high < x.high;
    return low < x.low;
}

// occó solution ;)
std::ostream &operator<<(std::ostream &iostream, const BigInt &x) {
    return iostream << "[" << x.high << "," << x.low << "]";
}
