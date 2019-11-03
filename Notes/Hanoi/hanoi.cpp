#include <iostream>
#include <vector>

using std::cout;

void near(int n, int f, int t, int x);
void far(int n, int f, int t, int x);

void hanoi(int n, int f, int t, int x) {
	if (n == 0) return;
	hanoi(n-1,f,x,t);
	cout << f << " ==> " << t << "\n";
	hanoi(n-1,x,t,f);
}

// void near(int n, int f, int t, int x) { // t is 1 move from f
// 	if (n == 0) return;
// 	far(n-1, f,x,t);
// 	cout << f << " ==> "<< x << "\n";

// }
// void far(int n, int f, int t, int x) { // t is 2 moves from f
// 	if (n == 0) return;
// 	far(n-1, f, t, x);
// 	cout << f << " ==> "<< t << "\n";
// 	near(n-1, t, f, x);
// 	cout << f << " ==> "<< x << "\n";
// 	far(n-1, )

// }

// void circular_hanoi(int n, int f, int t, int x) {
// 	if (n == 0) return;
	

// }

int main() {
	hanoi(3,0,1,2);
	std::vector<int> a{1,2,3};
	for (int x:a) cout << x<<"\n";
	cout<<"swap:\n";
	std::swap(a[0],a[1]);
	for (int x:a) cout << x<<"\n";
	return 0;
}