#include <iostream>
#include <vector>
using std::vector;
using std::cout;

template<typename T>
void insertion_sort(vector<T>& a) {
	for (int i = 1; i < a.size(); ++i) {
		int t = i;
		while (t > 0 && a[t] < a[t-1]) {
			int s = a[t];
			a[t] = a[t-1];
			a[t-1] = s;
			t--;
		}
	}
}

int main() {
	vector<int> a{3,2,4,6};
	insertion_sort(a);
	for (int x : a) {
		cout << x << "\n";
	}
	return 0;
}