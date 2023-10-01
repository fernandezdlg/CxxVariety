#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXf;
using Eigen::VectorXf;
using std::cout;
using std::endl;


/// @brief Computes the solution of the linear system Ax = b using the preconditioned conjugate gradient method. The matrices A and M are expected to be symmetric and positive definite (SPD).
/// @param Minv is the inverse of the preconditioner matrix.
/// @param A is the matrix of the linear system.
/// @param b is the right hand side of the linear system.
/// @param x0 is the initial guess of the solution.
/// @param tol is the error tolerance of the method.
/// @param maxIter is the maximum number of iterations.
/// @return Estimation of the solution x.
VectorXf precondConjugateGradient(const MatrixXf& Minv, const MatrixXf& A, const VectorXf& b, const VectorXf& x0, const float& tol, const u_int& maxIter)
{
    const float err = 1e-14 * b.norm();

    VectorXf x = x0;
    VectorXf r = b - A * x;
    VectorXf z = Minv * r;
    VectorXf p = z;

    float alpha = 1;
    VectorXf ri = r;
    VectorXf zi = z;
    uint32_t j = 0;
    while (r.norm() > err && j < maxIter)
    {
        alpha = ri.dot(zi) / (p.dot(A * p));
        x += alpha * p;
        r -= alpha * A * p;
        z = Minv * r;
        p = z + (r.dot(z) / ri.dot(zi)) * p;
        j++;
        ri = r;
        zi = z;
    }
    return x;
}


int main()  // int argc, char* argv[])
{
    MatrixXf A(3, 3); 
    A << 1, 0, 0,
         0, 1, 0,
         0, 0, 1;
    VectorXf b(3);
    b << 1, 2, 3;
    VectorXf x0(b.size()); 
    x0.setZero();

    MatrixXf Minv(3,3); // trivial preconditioner
    Minv << 1, 0, 0,
            0, 1, 0,
            0, 0, 1;

    float tol = 1e-14 * b.norm();
    u_int maxIter = 10000;

    VectorXf x = precondConjugateGradient(Minv, A, b, x0, tol, maxIter);
    cout << "This is the initial vector x0 : " << endl;
    cout << x0 << endl;
    cout << "This is the found solution vector x : " << endl;
    cout << x << endl;
    cout << "This is the vector b : " << endl;
    cout << b << endl;

    return 0;
}