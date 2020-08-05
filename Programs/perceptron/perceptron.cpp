#include <vector>

using std::vector;

using VD = vector<double>;

VD perceptron(vector<VD>& a, const VD& y, const int IT_MAX);

double dot_product(VD& a, VD& b) {
	double s = 0;
	for (int i = 0; i < a.size(); ++i) {
		s += (a[i] * b[i]);
	}
	return s;
}

int sgn(double s) {
	return s > 0 ? 1 : -1;
}

VD perceptron(vector<VD>& a, const VD& y, const int IT_MAX){
	VD weight(a.size()+1, 0);
	double learning_rate = 0.5;
	for (int iterations = 0; iterations < IT_MAX; ++iterations) {
		for (int i = 0; i < a.size(); ++i) {
			VD x = a[i];
			double update = learning_rate * (y[i] - sgn(dot_product(x,weight)));
			for (int j = 0; j < weight.size()-1; ++j) {
				weight[j] += update * x[j];
			}
			weight[weight.size()-1] = update;
		}
	}
	for (int i = 0; i < a.size(); ++i) {
		VD x = a[i];
		if (y[i] != sgn(dot_product(x,weight))) {
			return {};
		}
	}
	return weight;
}