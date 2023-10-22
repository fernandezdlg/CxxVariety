#include <iostream>
#include <Eigen/Dense>
#include <mgl2/mgl.h>

using namespace Eigen;

/// @brief Performs the cubic spline interpolation of the data with abscissae T and ordinatae Y. Each column represents the coefficients of the cubic polynomial on a subinterval. Assumes T is sorted, and has no repeated elements and T.size() == Y.size().
/// @param T is the vector of abscissae
/// @param Y is the vector of ordinatae
/// @return the (4, n) spline coefficients matrix
MatrixXd cubicSpline(const VectorXd &T, const VectorXd &Y) {
	int n = T.size() - 1; // T and Y have length n+1

	VectorXd h = T.tail(n) - T.head(n); // vector of lengths of subintervals

	// build the matrix of the linear system associated to the second derivatives
	MatrixXd A = MatrixXd::Zero(n-1, n-1);
	A.diagonal()   = (T.segment(2,n-1) - T.segment(0,n-1))/3;
	A.diagonal(1)  = h.segment(1,n-2)/6;
	A.diagonal(-1) = h.segment(1,n-2)/6; 

	// build the vector of the finite differences of the data Y
	VectorXd dydt = (Y.tail(n) - Y.head(n)).cwiseQuotient(h); 
				
	// right hand side vector for the system with matrix A
	VectorXd rhs = dydt.tail(n-1) - dydt.head(n-1);

	// solve the system and fill vector of second derivatives
	VectorXd sigma(n+1);
	sigma.segment(1,n-1) = A.fullPivLu().solve(rhs);
	sigma(0) = 0; // "simple" boundary conditions
	sigma(n) = 0; // "simple" boundary conditions

	// build the spline matrix with polynomials' coefficients
	MatrixXd abcd(4, n);
	abcd.row(0) = Y.head(n);
	abcd.row(1) = dydt - h.cwiseProduct(2*sigma.head(n) + sigma.tail(n))/6;
	abcd.row(2) = sigma.head(n)/2;
	abcd.row(3) = (sigma.tail(n) - sigma.head(n)).cwiseQuotient(6*h);

	return abcd;
}

/// @brief Evaluates the cubic spline S in the points evalT. Assumes T is sorted with no repetitions
/// @param S is the (4, n) spline coefficients matrix
/// @param T is the vector of abscissae
/// @param evalT is the vector of points where to evaluate the spline
/// @return the vector evalY, where evalY(i) is the value of the spline in evalT(i)
VectorXd evalCubicSpline(const MatrixXd &S, const VectorXd &T, const VectorXd &evalT) {
	int n = evalT.size();
	VectorXd evalY(n);

    size_t j = 0;
    for (int i=0; i<n; i++){
        while (evalT(i) > T(j+1)) j++;
        double x = evalT(i) - T(j);
        evalY(i) = S(0,j) + x*(S(1,j) + x*(S(2,j) + x*S(3,j)));
    }

	return evalY;
}

int main() {
	VectorXd T(9);
	VectorXd Y(9);
    
    // Simple example:
    // T << 0, 1, 2, 3, 4, 5, 6, 7, 8;
    // Y << 0, 1, 0, 1, 0, 1, 0, 1, 0;
	
	// Another example:
	T << 0, 0.4802, 0.7634, 1, 1.232, 1.407, 1.585, 1.879, 2;
	Y << 0., 0.338, 0.7456, 0, -1.234, 0 , 1.62, -2.123, 0;

	int len = 1 << 9; // number of points to plot
	VectorXd evalT = VectorXd::LinSpaced(len, T(0), T(T.size()-1));
	VectorXd evalY = evalCubicSpline(cubicSpline(T, Y), T, evalT);

    // Plotting
 	mglData datx, daty;
	datx.Link(evalT.data(), len);
	daty.Link(evalY.data(), len); 
	mglGraph gr;
	gr.SetRanges(0, 3, -3, 3);
	gr.Plot(datx, daty, "0");
	gr.WriteFrame("spline.eps");
}
	
