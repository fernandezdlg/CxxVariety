#include <iostream>
#include <Eigen/Eigen>

using namespace Eigen;
using std::cout;
using std::endl;

// This is a modified example from Eigen's ConjugateGradient class documentation 
// Ref: https://eigen.tuxfamily.org/dox/classEigen_1_1ConjugateGradient.html
int main()
{
    // initialize parameters
    int                  n = 3;
    VectorXd             x(n), b(n);
    SparseMatrix<double> A(n, n);
    
    // fill A and b
    A.insert(0, 0) = 1;
    A.insert(1, 1) = 1;
    A.insert(2, 2) = 1;
    b = VectorXd::Random(n);

    // solve Ax = b
    ConjugateGradient<SparseMatrix<double>, Lower | Upper> cg;
    cg.compute(A);
    x = cg.solve(b);
    cout << "#iterations:     " << cg.iterations() << endl;
    cout << "estimated error: " << cg.error() << endl;
    
    cout << "This is b : " << endl;
    cout << b << endl;
    cout << "This is the found x : " << endl;
    cout << x << endl;

    return 0;
}