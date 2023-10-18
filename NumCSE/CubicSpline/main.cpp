#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <cassert>

using namespace std;
using namespace Eigen;


MatrixXd cubicSplines(const VectorXd &T, const VectorXd &Y){
    // Input: T, Y
    // Returns: MatrixXd M containing the cubic spline coefficients to interpolate (T, Y)

    // Check if T and Y have the same size
    assert(T.size() == Y.size() && "T and Y must have the same size");
    
    int n = T.size();
    VectorXd h = T.tail(n) - T.head(n); // h_i = T_i+1 - T_i == dx

    // Construct tridiagonal matrix A = d^2Y/dx^2
    MatrixXd A = MatrixXd::Zero(n-1, n-1);
    A.diagonal() = (T.segment(2, n-1) - T.segment(0, n-1))/3; // A_ii = (T_i+1 - T_i-1)/3
    A.diagonal(1) = h.segment(1,n-2)/6; // A_i,i+1 = h_i/6
    A.diagonal(-1) = h.segment(1,n-2)/6; // A_i+1,i = h_i/6

    // Construct dY / dx
    VectorXd dYdh = (Y.tail(n) - Y.head(n)).cwiseQuotient(h);

    // Construct d^2Y/dx^2
    VectorXd d2Ydh2 = dYdh.tail(n-1) - dYdh.head(n-1);
    
}


int main() {
    // Input
    VectorXd T = VectorXd::LinSpaced(5, 0, 1);
    VectorXd Y = VectorXd::LinSpaced(5, 0, 1);
    
    // Returns

    return 0;
}