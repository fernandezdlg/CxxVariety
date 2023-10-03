#include <cassert>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

using std::cout;
using std::endl;

using Eigen::MatrixX2d;
using Eigen::MatrixXd;
using Eigen::Vector2d;
using Eigen::VectorXd;

/// @brief Function that performes the least squares solution to the overdetermined system Ax = b
/// via the normal equations
/// @param A is the matrix of the system
/// @param x is the vector that will be solved for
/// @param b is the right hand side of the system
void leastSquares(const MatrixXd& A, VectorXd& x, const VectorXd& b)
{
    assert(x.size() == A.cols() && "x must have the same number of elements as A has columns");
    x = (A.transpose() * A).fullPivLu().solve(A.transpose() * b);
}


int main()
{
    // =========================
    // Example: Solve argmin_{\alpha,\beta} || T*z - c ||_2
    // where T is a tridiagonal matrix with alpha on the diagonal, and beta on the off-diagonals.
    //
    // The problem can be reformulated by introducing a vector x = [alpha; beta] and matrix A
    // A = [[ z0, z1 ],[ z2, z1 + z3 ],[ ... ],... ]
    // such that T*z = A*x. This becomes is a least squares problem given the overdetermination of
    // the linear system, and can be solved by solving the normal equations T.transpose * T * x =
    // T.transpose * c
    // =========================

    int n = 3;
    // initialize z, c randomly
    VectorXd z(n);
    VectorXd c(n);
    z.setRandom(n);
    c.setRandom(n);

    // initialize A
    MatrixXd A(z.size(), 2);
    // ``Densified'' tridiagonal matrix:
    A.col(0) = z;
    A(0, 1)  = z(1);
    for (int i = 1; i < z.size() - 1; i++)
    {
        A(i, 1) = z(i - 1) + z(i + 1);
    }
    A(z.size() - 1, 1) = z(z.size() - 2);
 
    // initialize x
    VectorXd x(A.cols());
    leastSquares(A, x, c);

    cout << "Least squares determines" << endl;
    cout << "alpha = " <<  x(0) << ", beta = " << x(1) << endl;
    cout << "=> argmin_{alpha,beta} ||T z - c||_2 = " << endl;
    cout << (A * x - c).norm() << endl;
    cout << "where T z = " << endl;
    cout << A * x << endl;
    cout << "and c = " << endl;
    cout << c << endl;

    return 0;
}