#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


struct Newton
{
    Newton(const VectorXd& x)
    : _x(x)
    , _a(x.size())
    {
    }

    void Interpolate(const VectorXd& y)
    {
        _a = y;
        for (int k = 1; k < _x.size(); k++)
        {
            for (int i = _x.size() - 1; i >= k; i--)
            {
                _a(i) = (_a(i) - _a(i - 1)) / (_x(i) - _x(i - k));
            }
        }
    }

    double operator()(double t) const
    {
        double y = _a(_x.size() - 1);
        for (int i = _x.size() - 2; i >= 0; i--)
        {
            y = _a(i) + (t - _x(i)) * y;
        }
        return y;
    }

// private:
    VectorXd _x;  // nodes
    VectorXd _a;  // interpolating coefficients
};


struct Lagrange
{
    Lagrange(const VectorXd& x)
    : _x(x)
    , _l(x.size())
    , _y(x.size())
    {
        int n = _x.size();
        for (int j = 0; j < n; ++j)
        {
            double dw = 1.0;
            for (int i = 0; i < n; ++i)
            {
                if (i != j)
                    dw *= _x(j) - _x(i);
            }
            _l(j) = 1.0 / dw;
        }
    }

    void Interpolate(const VectorXd& y)
    {
        _y = y;
        // for (int i = 0; i < _x.size(); i++)
        // {
        //     _l(i) = 1;
        //     for (int j = 0; j < _x.size(); j++)
        //     {
        //         if (i != j)
        //             _l(i) *= (_x(i) - _x(j));
        //     }
        //     _l(i) = _y(i) / _l(i);
        // }
    }

    double operator()(double t) const
    {
        int      n = _x.size();
        VectorXd L(n);
        double   wx = 1.0;
        for (int i = 0; i < n; ++i)
        {
            wx *= t - _x(i);
        }
        for (int i = 0; i < n; ++i)
        {
            L(i) = wx * _l(i) / (t - _x(i));
        }
        return _y.dot(L);
    }

// private:
    VectorXd _x;  // nodes
    VectorXd _l;  // interpolating weights
    VectorXd _y;  // interpolating coefficients
};


// Runge function
VectorXd r(const VectorXd& x) { return (1.0 / (1.0 + 25.0 * x.array() * x.array())).matrix(); }


int main()
{
	int n = 5;
	Eigen::VectorXd x;
	x.setLinSpaced(5, -1.0, 1.0);
	Eigen::VectorXd y = r(x);

	Newton p(x);
	p.Interpolate(y);
    cout << "Correct result: p._a = [0.0384615, 0.198939, 1.5252, -3.31565, 3.31565], Newton Interpolation method gives: \n" << p._a << endl;

	Lagrange q(x);   
	q.Interpolate(y);
    cout << "Correct result: q._l = [0.666667, -2.66667, 4, -2.66667, 0.666667], Lagrange Interpolation method gives: \n" << q._l << endl;

	// Compute difference of p and q.
	int m = 22;
	double offset = 0.08333333333;
	x.setLinSpaced(m, -1.0 + offset, 1.0 - offset);
	double norm2 = .0;
	for (int i = 0; i < m; ++i) {
		double d = p(x(i)) - q(x(i));
		norm2 += d * d;
	}

	// By uniquenss of the interpolation polynomial, we expect p = q.
	std::cout << "The norm2 diff of both polinomials gives: " << norm2 << std::endl;
    cout << "This should be zero" << endl;
	return 0;
}
