#include <utility>
#include <god.hpp>

namespace god {

    int incr(const int& n) {
        return n + 1;
    }

    int decr(const int& n) {
        return n - 1;
    }

    int add(const int& firstAddend, const int& secondAddend) {
        return firstAddend + secondAddend;
    }

    int subtract(const int& minuend, const int& subtrahend) {
        return minuend - subtrahend;
    }

    int multiply(const int& multiplicand, const int& multiplier) {
        return multiplicand * multiplier;
    }

    std::pair<int, int> divide(const int& dividend, const int& divisor) {
        return { (dividend / divisor), (dividend % divisor) };
    }

}
