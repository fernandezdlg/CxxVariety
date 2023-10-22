#include <iostream>
#include <vector>
#include <functional>

using namespace std;

template<typename T> // T is float or double
T GaussLegendre5(const function<T(T)> &f, T a, T b){
	// nodes for 5th order quadrature on [-1,1]
	const vector<T> c = { 
		-0.90617984593,
		-0.53846931010,
		0.0,
		0.53846931010,
		0.90617984593 
	};
	// weights for 5th order quadrature on [-1,1]
	const vector<T> w = {
		0.23692688505,
		0.47862867049,
		0.56888888888,
		0.47862867049,
		0.23692688505 
	};
	
	T d = b - a;
	vector<T> cT;
	for (auto &cj : c){
		// Affine transformation of the nodes
		cT.push_back( d * (cj + 1.0) / 2.0 + a );
	}

	// Perform quadrature sum
	T I = 0.0;
	for (size_t i=0; i<c.size(); i++)
		I += w[i] * f(cT[i]);

	// Affine transformation of weights
	return I * d/2;
}


template<typename T> // T is float or double
T CompositeSimpson(const function<T(T)> &f, const vector<T> &x){
	size_t m = x.size() - 1;

	// constant terms
	T q = 1.0 / 6.0 * ( (x[1] - x[0]) * f(x[0]) + (x[m] - x[m - 1]) * f(x[m]) );

	// terms with coefficient 1/3
	for (size_t j = 1; j < m; ++j) {
		q += 1.0 / 6.0 * (x[j + 1] - x[j - 1]) * f(x[j]);
	}
	// terms with coefficient 2/3
	for (size_t j = 1; j <= m; ++j) {
		q += 2.0 / 3.0 * (x[j] - x[j - 1]) * f((x[j] + x[j - 1]) / 2);
	}

	return q;
}

template<typename T> // T is float or double
vector<T> linspace(T a, T b, size_t N){
	vector<T> x;
	for (size_t i=0; i<N; i++)
		x.push_back(a + (b-a)/N * i);
	return x;
}

int main() {
	// Define the function to integrate
	auto f = [](double x) -> double { return x*x; };
	double a = -2.0;
	double b = 1.5;

	auto I = GaussLegendre5<double>(f, a, b);

	cout << "The result of GaussLegendre5 is: " << I << endl;

	auto x = linspace(a, b, 100);

	auto I2 = CompositeSimpson<double>(f, x);

	cout << "The result of CompositeSimpson is: " << I2 << endl;

	cout << "Analytical integration is: " << b*b*b/3 - a*a*a/3 << endl;

	return 0;
}
	
