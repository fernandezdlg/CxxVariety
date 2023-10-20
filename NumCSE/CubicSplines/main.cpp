#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

MatrixXd cubicSpline(const VectorXd &T, const VectorXd &Y) {


    // Implement:
    int n = T.size() - 1; // number of *intervals*
    VectorXd h = T.tail(n) - T.head(n); // interval lengths h_j = t_j - t_j-1

    // TODO: perhaps a tridiagonal representation for A would be better?
    MatrixXd A = MatrixXd::Zero(n-1, n-1); // matrix of linear system 
    A.diagonal(  ) = (h.head(n-1) + h.tail(n-1))/3; // main diagonal  // h_j + h_j+1 /3
    A.diagonal( 1) = h.segment(1,n-2)/6;            // upper diagonal // h_j+1 /6
    A.diagonal(-1) = h.segment(1,n-2)/6;            // lower diagonal // h_i+1 /6

    VectorXd dydt = (Y.tail(n) - Y.head(n)).cwiseQuotient(h); // dydt_j = y_j - y_j-1 / h_j
    
    VectorXd rhs = dydt.tail(n-1) - dydt.head(n-1); // rhs of linear system

    // Solve linear system:
    VectorXd sigma(n+1); // s''
    sigma.segment(1,n-1) = A.fullPivLu().solve(rhs); // [A][sigma] = [rhs]
    sigma(0) = 0; // open boundary condition
    sigma(n) = 0; // open boundary condition

    // Compute spline coefficients:
    MatrixXd abcd(4, n);
    // a_j = y_j-1
    // b_j = dydt_j - h_j * (2 sigma_j-1 + sigma_j)/6
    // c_j = sigma_j-1 / 2
    // d_j = (sigma_j - sigma_j-1) / 6 h_j
    abcd.row(0) = Y.head(n); 
    abcd.row(1) = dydt - h.cwiseQuotient(2*sigma.head(n) + sigma.tail(n))/6;
    abcd.row(2) = sigma.head(n)/2;
    abcd.row(3) = (sigma.tail(n) - sigma.head(n)).cwiseQuotient(6*h); // s'' = (s''_i+1 - s''_i) / 6 dt_i+1

    return abcd;
}



int main() {


    return 0;
}