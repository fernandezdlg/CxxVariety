#include <iostream>
#include <functional>
#include <Eigen/Dense>

using Eigen::MatrixXf;
using Eigen::VectorXf;
using std::cout;
using std::endl;


/// @brief Computes the solution that minimizes a non linear function using the non-linear conjugate
/// gradient method. The function is expected to be well-behaved.
/// @param func is the function to be minimized.
/// @param dFunc is the function that computes the gradient of the function to be minimized.
/// @param ddFunc is the function that computes the Hessian of the function to be minimized.
/// @param x0 is the initial guess of the solution.
/// @param tolCG is the error tolerance of the conjugate gradient part.
/// @param tolNR is the error tolerance of the Newton-Raphson part.
/// @param maxIterCG is the maximum number of conjugate gradient iterations.
/// @param maxIterNR is the maximum number of Newton-Raphson iterations.
/// @return The estimation of the solution.
VectorXf nonLinConjugateGradient(std::function<VectorXf(VectorXf)> func,
                                 std::function<VectorXf(VectorXf)> dFunc,
                                 std::function<MatrixXf(VectorXf)> ddFunc,
                                 const VectorXf&                   x0,
                                 const float&                      tolCG,
                                 const float&                      tolNR,
                                 const u_int&                      maxIterCG,
                                 const u_int&                      maxIterNR)
{
    u_int       kCG = 0;
    u_int       kNR = 0;
    const u_int n   = x0.size();
    u_int       kN  = 0;

    VectorXf r = -dFunc(x0);
    VectorXf d = r;

    float    deltaNew = r.dot(r);
    float    delta0   = deltaNew;
    float    deltaD, alpha, deltaOld, beta;
    VectorXf x;

    while (kCG < maxIterCG && deltaNew > tolCG * tolCG * delta0)
    {
        kNR    = 0;
        deltaD = d.dot(d);
        while (kNR < maxIterNR && alpha * alpha * deltaD > tolNR * tolNR)
        {
            alpha = -dFunc(x0).dot(d) / d.dot(ddFunc(x0) * d);
            x     = x + alpha * d;
            kNR++;
        }
        r        = -dFunc(x0);
        deltaOld = deltaNew;
        deltaNew = r.dot(r);
        beta     = deltaNew / deltaOld;
        d        = r + beta * d;
        kCG++;
        if (kN == 50 || r.dot(d) <= 0)
        {
            d  = r;
            kN = 0;
        }
        kCG++;
    }
    return x;
}


/// @brief Function to be fed in the non linear conjugate gradient method.
/// @param x is the input vector.
/// @return The output vector.
VectorXf func(VectorXf x)
{
    // Simple non linear function
    VectorXf y(x.size());
    y(0) = x(0) * x(0) + x(1) * x(1) - 1;
    y(1) = x(0) * x(0) - x(1) * x(1) - 1;
    return y;
}

VectorXf dfunc(VectorXf x)
{
    // Simple non linear function
    VectorXf y(x.size());
    y(0) = x(0) * x(0) + x(1) * x(1) - 1;
    y(1) = x(0) * x(0) - x(1) * x(1) - 1;
    return y;
}

MatrixXf ddfunc(VectorXf x)
{
    // Simple non linear function
    VectorXf y(x.size());
    y(0) = x(0) * x(0) + x(1) * x(1) - 1;
    y(1) = x(0) * x(0) - x(1) * x(1) - 1;
    return y * y.transpose();
}


int main()  // int argc, char* argv[])
{   
    VectorXf x0(3);
    x0.setZero();

    float tol     = 1e-2;
    int   maxIter = 1000;

// FIXME: Sometimes Assertion failed: (aLhs.rows() == aRhs.rows() && aLhs.cols() == aRhs.cols()), function CwiseBinaryOp, file CwiseBinaryOp.h, line 116. Other times, the program runs but fails to print x to output.
    VectorXf x = nonLinConjugateGradient(func, dfunc, ddfunc, x0, tol, tol, maxIter, maxIter);

    cout << "This is the initial vector x0 : " << endl;
    cout << x0 << endl;
    cout << "This is the found solution vector x : " << endl;
    cout << x << endl;

    return 0;
}