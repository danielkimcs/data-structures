#include <iostream>
#include <random>

std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_int_distribution<unsigned long> dis;

template<typename T>
struct Node {
	T key;
	unsigned long priority;
	Node *left, *right;

	Node(T key) {
		this->key = key;
		this->priority = dis(gen);
		this->left = nullptr;
		this->right = nullptr;
	}
};

template<typename T>
class Treap {
public:
	Treap(T key);
	~Treap();
	Node<T>* rightRotate(Node<T>*);
	Node<T>* leftRotate(Node<T>*);
	Node<T>* insert(Node<T>*, T);
	Node<T>* remove(Node<T>*, T);
	Node<T>* find(Node<T>*, T);
	void destroy_recursive(Node<T>);
private:
	Node<T> root;
};

template<typename T>
Treap<T>::Treap(T key) {
	root = Node<T>(key);
}

template<typename T>
Node<T>* Treap<T>::rightRotate(Node<T> *y) { 
    Node<T> *x = y->left;
    Node<T> *z = x->right; 
    x->right = y; 
    y->left = z; 
    return x; 
}

template<typename T>
Node<T>* Treap<T>::leftRotate(Node<T> *x) { 
    Node<T> *y = x->right;
    Node<T> *z = y->left; 
    y->left = x; 
    x->right = z; 
    return y; 
} 

template<typename T>
Node<T>* Treap<T>::insert(Node<T>* rt, T key) { 
    if (!rt) return Node<T>(key); 
    if (key <= rt->key) { 
        rt->left = insert(rt->left, key); 
        if (rt->left->priority > rt->priority) 
            rt = rightRotate(rt); 
    } else {  
        rt->right = insert(rt->right, key); 
        if (rt->right->priority > rt->priority) 
            rt = leftRotate(rt); 
    } 
    return rt; 
} 

template<typename T>
Node<T>* Treap<T>::remove(Node<T>* rt, T key) {
    if (rt == NULL) return rt; 
    if (key < rt->key) 
        rt->left = remove(rt->left, key); 
    else if (key > rt->key) 
        rt->right = remove(rt->right, key);
    else { // key == rt->key
    	if (rt->left == NULL) { 
	        Node<T> *temp = rt->right; 
	        delete(rt); 
	        rt = temp;
	    } else if (rt->right == NULL) { 
	        Node<T> *temp = rt->left; 
	        delete(rt); 
	        rt = temp;
	    } else {
	    	if (rt->left->priority < rt->right->priority) { 
		        rt = leftRotate(rt); 
		        rt->left = remove(rt->left, key); 
		    } else { 
		        rt = rightRotate(rt); 
		        rt->right = remove(rt->right, key); 
		    }
		}
	}
  
    return rt;
}

template<typename T>
Node<T>* Treap<T>::find(Node<T>* rt, T key) {
    if (rt == NULL || rt->key == key) return rt;
    if (rt->key < key) return find(rt->right, key);
    else return find(rt->left, key); 
}

template<typename T>
void Treap<T>::destroy_recursive(Node<T> n) {
	if (n) {
		destroy_recursive(n->left);
		destroy_recursive(n->right);
		delete n;
	}
}

template<typename T>
Treap<T>::~Treap() {
	destroy_recursive(root);
}
