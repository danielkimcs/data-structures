#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using std::cout;
using std::ostream;
using std::endl;
using std::string;
using std::vector;
using std::cerr;
using VD = vector<double>;
const int MAX = 100;
const double EPS = 1e-7;

class Poly {
public:
	Poly(const VD& coeffs): c(coeffs) {}
	double operator()(double x) const {
		double val = 0;
		for (int i = c.size() - 1; i >= 0; --i) {
			val = c[i] + val * x;
		}
		return val;
	}
	Poly differentiate() const {
		VD new_c(c.size()-1, 0);
		for (int i = 1; i < c.size(); ++i) {
			new_c[i-1] = c[i]*i;
		}
		return Poly(new_c);
	}
private:
	VD c;
};

double newton_solve(double x0, const Poly& f) {
	Poly df = f.differentiate();
	for (int i = 0; i < MAX; ++i) {
		double dx = f(x0) / df(x0);
		if (fabs(df(x0)) < EPS * fabs(df(x0))) { cerr << "Error\n"; }
		if(fabs(dx) < EPS) return x0;
		x0 -= dx;
	}
	cerr << "Max iterations exceeded!\n";
	return 0;
}

int main() {
	Poly q(VD{1,-4,3,0,0,4});
	cout << q(4) << endl;
	cout << newton_solve(2, q) << endl;
	cout << newton_solve(0, q) << endl;
	cout << newton_solve(-2, q) << endl;
	return 0;
}