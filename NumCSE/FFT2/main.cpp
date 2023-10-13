#include <cassert>
#include <iostream>
#include <vector>

#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>

using namespace std;
using Eigen::MatrixXcd;
using Eigen::MatrixXd;
using Eigen::MatrixBase;
using Eigen::FFT;


// Implement FFT-2D on Eigen MatrixXd

template <typename Scalar> void fft2(MatrixXcd &C, const MatrixBase<Scalar> &Y) {
    // Y_{m, n}
    int m = Y.rows();
    int n = Y.cols();

    // C_{m, n}
    C.resize(m, n);
    MatrixXd tmp(m, n);

    FFT<double> fft;

     // FIXME: Something shady here
    // Perform fft on rows of Y
    for (int i = 0; i < m; i++) {
        // tmp.row(i) = fft.fwd(Y.row(i)).transpose();
        fft.fwd(tmp.row(i), Y.row(i));
    }
     // FIXME: Something shady here
    // Perform fft on columns of tmp
    for (int j = 0; j < n; j++) {
        // tmp.col(j) = fft.fwd(tmp.col(j));
        fft.fwd(tmp.col(j), Y.col(j));
    }
}


int main(){

    MatrixXcd C(3,3);
    C << 1, 2, 3,
         4, 5, 6, 
         7, 8, 9;

    MatrixXcd Y(3,3);
    fft2(Y, C);
    cout << Y << endl;

    return 0;
}