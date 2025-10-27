#include <utility>
#include <god.hpp>

//!
//! @brief Main namespace for basic arithmetic operations.
//! * this namespace provides fundamental integer arithmetic functions.
//! 
namespace god {
    //!
    //! @brief Increments an integer by one.
    //! * @param n The integer to be incremented.
    //! @return The result of n + 1.
    //!
    int incr(const int& n) {
        return n + 1;
    }

    //!
    //! @brief Decrements an integer by one.
    //! * @param n The integer to be decremented.
    //! @return The result of n - 1.
    //!
    int decr(const int& n) {
        return n - 1;
    }

    //!
    //! @brief Adds two integers.
    //! * @param firstAddend The first number to add.
    //! @param secondAddend The second number to add.
    //! @return The sum of the two integers.
    //!
    int add(const int& firstAddend, const int& secondAddend) {
        return firstAddend + secondAddend;
    }

    //!
    //! @brief Subtracts one integer from another.
    //! * @param minuend The number from which to subtract.
    //! @param subtrahend The number to subtract.
    //! @return The difference.
    //!
    int subtract(const int& minuend, const int& subtrahend) {
        return minuend - subtrahend;
    }

    //!
    //! @brief Multiply two integers.
    //! * @param multiplicand The first number to multiply.
    //! @param multiplier The number to multiply multiplicand by.
    //! @return The product.
    //!
    int multiply(const int& multiplicand, const int& multiplier) {
        return multiplicand * multiplier;
    }

    //!
    //! @brief Divide two integers, returning the quotient and remainder.
    //! * @param dividend The number to be divided.
    //! @param divisor The number by which to divide.
    //! @return A pair consisting of {quotient, remainder}.
    //!
    std::pair<int, int> divide(const int& dividend, const int& divisor) {
        return { (dividend / divisor), (dividend % divisor) };
    }

}
