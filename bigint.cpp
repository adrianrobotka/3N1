#include "bigint.h"
#include <cassert>

BigInt::BigInt() : big(0), little(0) {}

BigInt::BigInt(Unit little) : big(0), little(little) {}

BigInt::BigInt(Unit big, Unit little) : big(big), little(little) {}

BigInt::BigInt(const BigInt &a) : big(a.big), little(a.little) {}

/* This will be used only for multiplication by 3 */
BigInt &BigInt::operator*=(Unit x) {
    assert(x == 3);
    /**
     * The math:
     * x *= 3*x;
     * x <- 3*x mod (2^64)
     * There are 3 cases: Where the new value will be 3*x, 3*x-2^64 or 3*x-2^65
     * These can be differentiated by the new value mod 3.
     * Thus we can conclude what was the lost data
     */
    big *= 3;
    little *= 3;
    if (little % 3 == MINUS_UNIT_MOD_3)
        ++big;
    if (little % 3 == MINUS_2_UNIT_MOD_3)
        big += 2;
    return *this;
}

/* This will be used only for dividing by 2 */
BigInt &BigInt::operator>>=(Unit x) {
    assert(x == 1);
    little >>= 1;
    if ((big & 1) == 1)
        little |= UNIT_LARGEST_BIT;
    return *this;
}

BigInt &BigInt::operator++() {
    ++little;
    if (little == 0)
        ++big;
    return *this;
}

/* Used for calculating next assignment */
BigInt &BigInt::operator+=(const BigInt &x) {
    little += x.little;
    if (little < x.little) ++big;
    big += x.big;
    return *this;
}

/* Used for getting parity */
Unit BigInt::operator&(Unit x) {
    return little & x;
}

bool BigInt::operator>=(const BigInt &x) {
    if (big != x.big) return big >= x.big;
    return little >= x.little;
}

bool BigInt::operator==(const BigInt &x) {
    if (big != x.big) return big == x.big;
    return little == x.little;
}

bool BigInt::operator<=(const BigInt &x) {
    if (big != x.big) return big <= x.big;
    return little <= x.little;
}

bool BigInt::operator!=(const BigInt &x) {
    if (big != x.big) return big != x.big;
    return little != x.little;
}

bool BigInt::operator>(const BigInt &x) {
    if (big != x.big) return big > x.big;
    return little > x.little;
}

bool BigInt::operator<(const BigInt &x) {
    if (big != x.big) return big < x.big;
    return little < x.little;
}

// occó solution
std::ostream &operator<<(std::ostream &iostream, const BigInt &x) {
    return iostream << "[" << x.big << "," << x.little << "]";
}
