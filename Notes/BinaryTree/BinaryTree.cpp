#include <iostream>
#include <stack>
#include <vector>

using std::stack;
using std::vector;
using VI = vector<int>;

template<typename T>
struct BN {
    T key;
    int sz;
    BN<T>* left;
    BN<T>* right;
};

template<typename T>
int height(BN<T> *root) {
    if (root) return 1+ max(height(root->l),height(root->r));
    return -1;
}

template<typename T>
int size(BN<T> *root) {
    if (root) return 1+size(root->l)+size(root->r);
    return 0;
}

template<typename T>
BN<T>* find(BN<T>* root, int ord) {
	// if (root) return root;
	if (ord >= root->sz) return nullptr;
	if (root->l->sz > ord) {
		return find(root->l, ord);
	} else if (root->l->sz == ord) return root;
	else {
		return(root->r, root->sz - ord - 1);
	}
}

// template<typename T>
// struct It {
// 	stack<BN> s;
// 	BN<T> *node;
// }

// void next(It& it) {

// }

// Construct the binary tree that has unique keys,
// whose in-order and pre-order are specified by in and pre

template<typename T>
BN<T>* reconstruct(VI& in, VI& pre) {
	It 
}







