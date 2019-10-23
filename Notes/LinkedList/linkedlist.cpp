#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

template <typename T>
struct Link {
    explicit Link (const T& info, Link *next=nullptr) : info(info), next(next) {}
    ~Link() { delete next; } // deletes the whole list
    // If you just want to delete one element, set next=nullptr first and then delete next
    
    T info;
    Link *next;
};

Link<int>* reverse(Link<int>* head) {
	// Cannot copy to a vector and create new Links
	// Can't use new at all

	Link<int> *nxt = head->next;
	Link<int> *prev = nullptr;
	Link<int> *current = head;
	while (current != nullptr) {
		nxt = current->next;
		current->next = prev;
		prev = current;
		current = nxt;
	}
	return prev;
}

void print_list(Link<int>* head) {
	Link<int>* new_head = head;
	Link<int> *nxt = new_head->next;
	while (nxt) {
		cout << new_head->info << " ";
		nxt = nxt->next;
		new_head = new_head->next;
	}
	cout << new_head->info << " ";
	cout << std::endl;
}

int main() {
	Link<int> *head = new Link<int>(1);
	vector<int> vs {2,3,4};
	for (int i = 0; i < vs.size(); ++i) {
	    head = new Link<int>(vs[i], head);
	}
	print_list(head);
	print_list(reverse(head));
	
	return 0;
}