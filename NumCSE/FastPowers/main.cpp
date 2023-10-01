#include <cassert>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

/// @brief Function that computes a^b in O(b) time
/// @param a is the base
/// @param b is the exponent (must be integer)
/// @return The result of a^b
double slowPower(double a, int b)
{
    double result = 1.0;
    if (b < 0)
    {
        a = 1.0 / a;
        b = -b;
    }

    for (int i = 0; i < b; i++)
    {
        result *= a;
    }
    return result;
}


/// @brief Function for the computation of a^b in O(log b)? time
/// @param a is the base
/// @param b is the exponent (must be integer)
/// @return The result of a^b
double power(double a, int b)
{
    double result = 1.0;
    if (b < 0)
    {
        a = 1.0 / a;
        b = -b;
    }

    while (b > 0)
    {
        if (b % 2 == 1)
        {
            result *= a;
        }
        a *= a;
        b /= 2;
    }
    return result;
}


/// @brief Function for the computation of a^b in O(log b) time using recursion
/// @param a is the base
/// @param b is the exponent (must be integer)
/// @return The result of a^b
double fastPower(double a, int b)
{
    double result = 1.0;
    if (b < 0)
    {
        a = 1.0 / a;
        b = -b;
    }

    if (b == 0)
        return 1;
    if (b == 1)
        return a;
    if (b & 1)  // binary operation for "b is Odd"
        return a * fastPower(a, b - 1);

    return fastPower(a * a, b / 2);
}


/// @brief Function that computes a^b in O(1) time
/// @param a is the base
/// @param b is the exponent (must be integer)
/// @return The result of a^b
double fastPower(double a, double b)
{
    return std::exp(b * std::log(a));  // sometimes the simplest calls are the best
}

int main()
{
    double err1, err2, err3;
    err1 = err2 = err3 = 0.0;
    int count          = 0;
    for (double a = 1; a < 100; a++)
    {
        for (int b = 0; b < 300; b++)
        {
            if (isinf(slowPower(a, b)) || isinf(fastPower(a, b)) || isinf(fastPower(a, (double) b))
                || isinf(std::pow(a, b)))
            {
                continue;
            }

            err1 += std::abs((slowPower(a, b) - std::pow(a, b)) / std::pow(a, b));
            err2 += std::abs((fastPower(a, b) - std::pow(a, b)) / std::pow(a, b));
            err3 += std::abs((fastPower(a, (double) b) - std::pow(a, b)) / std::pow(a, b));
            count++;
        }
    }
    cout << "Average error for slowPower: " << err1 / count << endl;
    cout << "Average error for fastPower: " << err2 / count << endl;
    cout << "Average error for fastPower(double): " << err3 / count << endl;
    cout << "Number of tests: " << count << endl;
    cout << "For reference, the machine error for a double is: " << std::numeric_limits<double>::epsilon() << endl;

    return 0;
}