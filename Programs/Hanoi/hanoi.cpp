#include <iostream>
#include <vector>
#include <utility>
#include <stack> // only used for test_hanoi

int MAX_TOWERS = 10;
int MAX_DISKS = 10000;

using std::cout;
using std::cin;
using std::vector;
using std::swap;

typedef vector<int> VI;
typedef vector<long> VL;
vector<VL> n_hanoi;
vector<VI> k_hanoi;
// Initialize n_hanoi and k_hanoi
void hanoi_init();
// Fill moves so that each element is a two-integer VI describing the move
// You may assume that, initially, aux[] = {0, 1, 2, ..., n_disks - 1}
void hanoi(vector<VI>& moves, int n_twrs, int n_dsks, VI& aux);

void hanoi_init() {
	for (int twr = 0; twr < MAX_TOWERS+1; ++twr) {
		n_hanoi.push_back(VL(MAX_DISKS+1));
		k_hanoi.push_back(VI(MAX_DISKS+1));
		for (int dsk = 0; dsk < MAX_DISKS+1; ++dsk) {
			if (twr < 2 || dsk == 0) n_hanoi[twr][dsk] = k_hanoi[twr][dsk] = 0;
			else {
				if (twr == 2) {
					if (dsk < 2) {
						n_hanoi[twr][dsk] = dsk;
					} else n_hanoi[twr][dsk] = -1;
				} else {
					if (dsk > 1) {
						n_hanoi[twr][dsk] = -1;
						for (long k = 1; k < dsk; ++k) {
							if (n_hanoi[twr - 1][dsk - k] == -1 || n_hanoi[twr][k] == -1) continue;
							long p = 2*n_hanoi[twr][k];
							if (p < 0) continue;
							long x = p + n_hanoi[twr - 1][dsk - k];
							if (x < 0) continue;
							if (n_hanoi[twr][dsk] == -1 || n_hanoi[twr][dsk] > x) {
								n_hanoi[twr][dsk] = x;
								k_hanoi[twr][dsk] = k;
							}
						}
					} else {
						n_hanoi[twr][dsk] = dsk;
						k_hanoi[twr][dsk] = 0;
					}
				}
			}
		}
	}
}

void hanoi(vector<VI>& moves, int n_twrs, int n_dsks, VI& aux) {
	int k = k_hanoi[n_twrs][n_dsks];
	if (n_dsks == 1) {
		moves.push_back({aux[0],aux[1]}); return;
	}
	swap(aux[1],aux[n_twrs-1]);
	hanoi(moves, n_twrs, k, aux);
	swap(aux[1],aux[n_twrs-1]);
	hanoi(moves, n_twrs-1, n_dsks-k, aux);
	swap(aux[0],aux[n_twrs-1]);
	hanoi(moves, n_twrs, k, aux);
	swap(aux[0],aux[n_twrs-1]);
}

bool test_hanoi(vector<vector<int>> ans,int n_dsks) {
    vector<std::stack<int>> t(10);
    for (int i = n_dsks;i > 0;i--) {
        t[0].emplace(i);
    }

    for (vector<int> move : ans) {
        if (t[move[0]].empty()) return false;
        if (!t[move[1]].empty()) {
            if (t[move[0]].top() >= t[move[1]].top()) return false;
        }
        t[move[1]].emplace(t[move[0]].top());
        t[move[0]].pop();
    }

    for (int i = 1;i <= n_dsks;i++) {
        if (t[1].top() != i) {
            return false;
        }
        t[1].pop();
    }
    return true;
}

int main() {
    hanoi_init();
    vector<VI> moves(0);
    vector<int> aux{0,1,2,3,4,5,6,7,8,9};
    for (int twrs = 3;twrs <= 10;twrs++) {
        for (int dsks = 1;dsks < 10000;dsks++) {
            if (n_hanoi[twrs][dsks] == -1 || n_hanoi[twrs][dsks] > 10000) {
                break;
            }
            moves.clear();
            hanoi(moves,twrs,dsks,aux);
            if (moves.size() != n_hanoi[twrs][dsks]) {
			cout << "Failed on " << twrs << "towers and " << dsks << " disks because the given moveset is not the same length as n_hanoi" << std::endl;
} else if (!test_hanoi(moves,dsks)) {
                cout << "Failed on " << twrs << "towers and " << dsks << " disks because the given moveset is not a valid solution" << std::endl;
            }
        }
    }
    cout << "everything is awesome" << std::endl; //note that this gets printed regardless of whether u passed or not

}
