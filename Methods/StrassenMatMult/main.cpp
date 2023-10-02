#include <cassert>
#include <iostream>
#include <vector>
#include <Eigen/Dense>

#define block11 block(0, 0, N / 2, N / 2)
#define block12 block(0, N / 2, N / 2, N / 2)
#define block21 block(N / 2, 0, N / 2, N / 2)
#define block22 block(N / 2, N / 2, N / 2, N / 2)

using std::cout;
using std::endl;

using Eigen::MatrixXf;
using Eigen::VectorXf;




/// @brief Simple function to determine whether N == 2^k for k integer.
/// @param N is the number to check.
/// @return True if N == 2^k for k integer, false otherwise.
bool isN2powK(int N) {
    return (N & (N - 1)) == 0;
}


MatrixXf strassenMult(const MatrixXf & A, const MatrixXf & B) {
    
    // Assert that A and B are square matrices of the same size N = 2^n, for n integer
    assert(A.rows() == A.cols() && "Matrix A must be square");
    assert(B.rows() == B.cols() && "Matrix B must be square");
    assert(A.rows() == B.rows() && "Matrices A and B must be of the same size");
    
    int N = A.rows();
    assert(isN2powK(N) && "Matrix size must be a power of 2");

    //Trivial case
    if (N == 1) {
        return A * B;
    }

    // Compute Strassen matrices recursively
    MatrixXf M1 = strassenMult(A.block11 + A.block22, B.block11 + B.block22);
    MatrixXf M2 = strassenMult(A.block21 + A.block22, B.block11);
    MatrixXf M3 = strassenMult(A.block11, B.block12 - B.block22);
    MatrixXf M4 = strassenMult(A.block22, B.block21 - B.block11);
    MatrixXf M5 = strassenMult(A.block11 + A.block12, B.block22);
    MatrixXf M6 = strassenMult(A.block21 - A.block11, B.block11 + B.block12);
    MatrixXf M7 = strassenMult(A.block12 - A.block22, B.block21 + B.block22);

    // Initialize matrix C and store in blocks
    MatrixXf C(N,N);
    C << M1 + M4 - M5 + M7,       M3 + M5,
              M2 + M4,        M1 - M2 + M3 + M6;

    return C;
}

int main(){
    int N = pow(2, 3);
    // Initialize matrices A, B
    MatrixXf A, B;
    
    // Initialize A, B to random integers
    A = MatrixXf::Random(N, N);
    B = MatrixXf::Random(N, N);

    // Compute C = A * B using Strassen's algorithm
    MatrixXf C = strassenMult(A, B);
    
    // Print result to compare with Eigen's matrix multiplication
    // cout << C << endl;
    // cout << endl;
    // cout << A*B << endl;
    
    assert (C.isApprox(A * B) && "Strassen's algorithm failed");
    cout << "Strassen's algorithm succeeded" << endl;

    return 0;
}