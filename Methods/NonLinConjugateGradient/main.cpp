#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXf;
using Eigen::VectorXf;
using std::cout;
using std::endl;

// TODO: Non-linear CG explicit impl. example
// Explicit implementation of the conjugate gradient method (probably suboptimal)
VectorXf conjugateGradient(MatrixXf& A, VectorXf& b, VectorXf& x0, float& tol, int& maxIter)
{
    VectorXf r = b - A * x0;
    if (r.norm() < tol)
    {
        return x0;
    }
    VectorXf p = r;
    VectorXf x = x0;
    VectorXf w = A * p;
    int      k = 0;
    float    alpha, beta, res;
    while (k < maxIter)
    {
        alpha = r.dot(r) / p.dot(w);
        x     = x + alpha * p;
        r     = r - alpha * w;
        res   = r.norm();
        if (res < tol)
        {
            break;
        }
        beta = r.dot(r) / p.dot(p);
        p    = r + beta * p;
        w    = A * p;
        k++;
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

    float tol     = 1e-2;
    int   maxIter = 1000;

    VectorXf x = conjugateGradient(A, b, x0, tol, maxIter);
    cout << "This is the initial vector x0 : " << endl;
    cout << x0 << endl;
    cout << "This is the found solution vector x : " << endl;
    cout << x << endl;
    cout << "This is the vector b : " << endl;
    cout << b << endl;

    return 0;
}