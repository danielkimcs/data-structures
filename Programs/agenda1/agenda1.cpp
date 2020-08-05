#include <iostream>
#include <vector>
#include <math.h>

using std::vector;
using std::cout;
using std::endl;
using VD = vector<double>;

VD find_ineq(double x0, double y0, double x1, double y1, double nx, double ny) {
	double a = y1-y0;
	double b = x0-x1;
	double c = x0*(y1-y0)-y0*(x1-x0);
	double d = a*nx+b*ny-c;
	if (d <= 0) return {a,b,c};
	else return {-a,-b,-c};
}

void f(vector<VD>& A, VD& b, VD& c, const VD& x, const VD& y) {
	vector<VD> ineqs;
	int l = x.size();
	for (int i = 0; i < l - 2; ++i) ineqs.push_back(find_ineq(x[i],y[i],x[i+1],y[i+1],x[i+2],y[i+2]));
	ineqs.push_back(find_ineq(x[l-2],y[l-2],x[l-1],y[l-1],x[0],y[0]));
	ineqs.push_back(find_ineq(x[l-1],y[l-1],x[0],y[0],x[1],y[1]));
	vector<VD> newA(l, VD(5));
	for (int i = 0; i < l; ++i) {
		newA[i][0] = ineqs[i][0];
		newA[i][1] = -ineqs[i][0];
		newA[i][2] = ineqs[i][1];
		newA[i][3] = -ineqs[i][1];
		newA[i][4] = sqrt(ineqs[i][0]*ineqs[i][0]+ineqs[i][1]*ineqs[i][1]);
	}
	VD newB(l);
	for (int i = 0; i < l; ++i) newB[i] = ineqs[i][2];
	c = {0,0,0,0,1};
	A = newA;
	b = newB;
}

int main() {
	vector<VD> A = {{}};
	VD b = {};
	VD c = {};
	VD x = {1,0,-3,0};
	VD y = {0,2,0,-4};
	f(A,b,c,x,y);
	cout << "A:"<<endl;
	for (int i = 0; i < A.size(); ++i) {
		for (int j = 0; j < A[i].size(); ++j) {
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
	cout << "B:"<<endl;
	for (int i = 0; i < b.size(); ++i) {
		cout << b[i] << " ";
	}
	return 0;
}