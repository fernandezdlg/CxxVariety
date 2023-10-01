#include <cassert>
#include <climits>
#include <cmath>
#include <float.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

/// @brief A templated factorial function iteratively computing n! for n integer.
/// @tparam T is the type of the input
/// @param n is the input
/// @return The value of n!
template<class T>
T factorial(T n)
{
    T result = 1;
    for (T i = 2; i <= n; i++)
    {
        result *= i;
    }
    return result;
}


/// @brief Templated function that returns the factorial as a double, input is rounded to nearest
/// integer.
/// @tparam T is the type of the input
/// @param n is the input
/// @return The value of n! as a double
template<class T>
double dblFactorial(T n)
{
    return factorial((double) round(n));
}


/// @brief Function to test the factorial function
/// @tparam T is the type of the input
/// @param max is the maximum value for datatype T
template<class T>
void test(T max)
{
    // Find the limit of the factorial function => limit according to datatype T
    T x = 1;
    while (max >= 1)
    {
        max /= ++x;
    }
    cout << "Factorial of " << x << " is NOT " << factorial(x) << "👎" << endl;
    cout << "Factorial of " << x - 1 << " is " << factorial(x - 1) << "👍" << endl;
}

int main()
{
    test(INT_MAX);
    test(LONG_MAX);
    test(DBL_MAX);
}