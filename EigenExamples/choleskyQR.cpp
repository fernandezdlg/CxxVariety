#include <cassert>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

using std::cout;
using std::endl;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::LLT;

/// @brief Function to perform the Cholesky decomposition of a matrix A
/// @param A is a symmetric positive definite matrix
/// @return the lower triangular matrix L such that A = L * L^T
MatrixXd cholesky(const MatrixXd &A) {
    return A.llt().matrixL();    
}

/// @brief Function to perform the A = Q * R decomposition via the Cholesky decomposition of A^T * A
/// @param A is a (m, n) matrix of ``full rank'': rank(A) = n
/// @param Q is a (m, n) matrix such that Q^T * Q = identity(n)
/// @param R is a (n, n) upper triangular matrix
void choleskyQR(const MatrixXd &A, MatrixXd &Q, MatrixXd &R) {
    LLT<MatrixXd> sol = (A.transpose() * A).llt();
    MatrixXd L = sol.matrixL();
    Q = L.triangularView<Eigen::Lower>().solve(A.transpose()).transpose(); // Q = A * (L^{-1}).transpose() = (L^{-1} * A).transpose()
    R = L.transpose(); // R = L.transpose()
}


/// @brief Function to perform the thin/reduced QR decomposition of matrix A
/// @param A is an (m, n) matrix
/// @param Q is a (m, m) matrix such that Q^T * Q = identity(m)
/// @param R is a (m, n) upper triangular matrix
void hhQR(const MatrixXd &A, MatrixXd &Q, MatrixXd &R) {
    Q = A.householderQr().householderQ();
    R = A.householderQr().matrixQR().triangularView<Eigen::Upper>();
    // one could also do:??
    // R = MatrixXd::Identity(std::min(A.rows(), A.cols()), A.rows()) * A.householderQr().matrixQR().triangularView<Eigen::Upper>();
}


int main()
{
    int n = 3;

    // Test cholesky
    MatrixXd A(3,3);
    A << 1, 2, 3,
         2, 5, 6,
         3, 6, 9;
    // Add ones to A(:,:)
    A = A + 1.1*MatrixXd::Ones(A.rows(), A.cols());
    // Make A symmetric
    A = A + A.transpose().eval(); // A = A + A.transpose() is incorrect due to aliasing

    MatrixXd L = cholesky(A);
    cout << "A = " << endl;
    cout << A << endl;
    cout << "L = " << endl;
    cout << L << endl;
    cout << "L * L^T = " << endl;
    cout << L * L.transpose() << endl;

    // Check Cholesky decomposition
    assert((L * L.transpose()).isApprox(A) && "Cholesky decomposition failed");

    // Perform QR decompositions
    int m = 4;
    MatrixXd B(m, n);
    B.setRandom(m, n);
    MatrixXd Qc;
    MatrixXd Q;
    MatrixXd Rc;
    MatrixXd R;
    choleskyQR(B, Qc, Rc);
    hhQR(B, Q, R);

    cout << "B = " << endl;
    cout << B << endl;
    cout << "Cholesky's: Qc * Rc = " << endl;
    cout << Qc * Rc << endl;    
    cout << "Householder's: Q * R = " << endl;    
    cout << Q * R << endl;

    // Check that the QR decompositions are equivalent
    
    assert((Qc * Rc).isApprox(B) && "QR decomposition via Cholesky decomposition failed");
    assert((Q * R).isApprox(B) && "Householder QR decomposition failed");

    assert((Qc.transpose() * Qc).isApprox(MatrixXd::Identity(Qc.cols(),Qc.cols())) && "Qc is not orthogonal");
    assert((Q.transpose() * Q).isApprox(MatrixXd::Identity(Q.cols(),Q.cols())) && "Q is not orthogonal");

    assert(R.isUpperTriangular() && "R is not upper triangular");
    assert(Rc.isUpperTriangular() && "Rc is not upper triangular");

    return 0;
}