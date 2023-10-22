#include <iostream>
#include <vector>

#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

double f1(const Vector2d &x) {
	return x(0) * x(0) + 2. * x(1) * x(1) - 1.;
}

double f2(const Vector2d &x) {
	return x(1) - x(0) * x(0);
}

Vector2d f(const Vector2d &x) {
	return Vector2d(f1(x), f2(x));
}

Matrix2d Jacobian(const Vector2d &x) {
	Matrix2d J;
	J << 2. * x(0), 4. * x(1), -2. * x(0), 1.;
	return J;
}

Vector2d Newton(Vector2d x, int n) {
	for (int i = 0; i < n; ++i) {
		x += Jacobian(x).fullPivLu().solve(-f(x));
	}
	return x;
}

IOFormat ShortFmt(StreamPrecision, DontAlignCols, ", ", ", ", "", "", "[", "]");

int main() {
	int n = 100;
	vector<Vector2d> startingPoints(3);

	startingPoints[0] = Vector2d(-1., 1.);
	startingPoints[1] = Vector2d(1., 1.);
	startingPoints[2] = Vector2d(-2., -2.);

	cout << "---------------------------" << endl;
	for (const Vector2d &x : startingPoints) {
		Vector2d y = Newton(x, n);
		cout << "   Starting point:  = " << x.format(ShortFmt) << endl;
		cout << "   Found solution:  = " << y.format(ShortFmt) << endl;
		cout << "   f(y) =? 0:         " << f(y).format(ShortFmt) << endl;
		cout << "---------------------------" << endl;
	}

	cout << "\nThe analytical solution is (+/- 1/sqrt(2), 1/2) : \n (+/- " << 1/sqrt(2) << ", " << 1.0/2.0 << ")" << endl;

	return 0;
}
