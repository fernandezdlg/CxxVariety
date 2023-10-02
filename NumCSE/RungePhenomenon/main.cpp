#include <cassert>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

using std::cout;
using std::endl;
using Eigen::MatrixXd;
using Eigen::VectorXd;


/// @brief Function that computes the Vandermonde matrix
/// @param x is the vector of sampling points
/// @param n is the degree of the approximating polynomial
/// @return The Vandermonde matrix MatrixXd V
MatrixXd vandermonde(const VectorXd &x, int n){
    int m = x.size();
    assert(m > 0 && "m must be positive");
    assert(n > 0 && "n must be positive");

    MatrixXd V(m, n);
    V.col(0) = VectorXd::Ones(m);
    for(int i = 1; i < n; i++){
        V.col(i) = V.col(i-1).cwiseProduct(x); // Vectorize
    }

    return V;
}


/// @brief This evaluates the Runge function
/// @param x are the sampling points
/// @return The vector of function evaluations y_i = r(x_i)
VectorXd runge(const VectorXd &x){
    // int m = x.size();
    // assert(m > 0 && "m must be positive");

    // VectorXd y(m);
    // for(int i = 0; i < m; i++){
    //     y(i) = 1.0 / (1.0 + 25.0 * x(i) * x(i));
    // }
    // return y;
    return (1.0 / (1.0 + 25.0 * x.array() * x.array())).matrix();
}


int main(){
    int m, n;
    n = 11;
    VectorXd x;
    VectorXd y;
    MatrixXd VM;
    VectorXd a(n);

    // Define the sampling points:
    m = n;
    x.setLinSpaced(m, -1.0, 1.0); // linspace

    y = runge(x);
    VM = vandermonde(x, n);

    // Solve the linear system VM * a = y
    a = VM.lu().solve(y);

    // Print coeffs a:
    cout << "Coefficients of the interpolating polynomial:" << endl;
    Eigen::IOFormat PythonFmt(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ";\n", "[", "]", "[", "]");
    cout << a.transpose().format(PythonFmt) << endl;


    // Repeat but with least squares
    m = 3*n;
    x.setLinSpaced(m, -1.0, 1.0); // linspace
    y = runge(x);
    VM = vandermonde(x, n);

    a = (VM.transpose() * VM).llt().solve(VM.transpose() * y);
    // Print coeffs a:
    cout << "Coefficients of the interpolating polynomial:" << endl;
    cout << a.transpose().format(PythonFmt) << endl;


    return 0;
}