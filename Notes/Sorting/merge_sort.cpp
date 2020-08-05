#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

template<typename K>
struct Link {
    K key;
    Link<K>* next;
};

template<typename K>
Link<K>* merge_sort(Link<K> *head) {
	if (head->next == nullptr) return head;
    int size = 0;
    Link<K> *cur = head;
    while (cur != nullptr) {
    	++size;
    	cur = cur->next;
    }
    int half = size/2-1;
    cur = head;
    int count = 0;
    while (count < half) {
    	count++;
    	cur = cur->next;
    }
    Link<K> *second_half = cur->next;
    cur->next = nullptr;
    Link<K> *fp = merge_sort(head);
    Link<K> *sp = merge_sort(second_half);
    Link<K> *new_merge;
    if (fp->key < sp->key) {
    	new_merge = fp;
    	fp = fp->next;
    } else {
    	new_merge = sp;
    	sp = sp->next;
    }
    Link<K> *new_head = new_merge;
    while (fp != nullptr && sp != nullptr) {
    	if (fp->key < sp->key) {
    		new_merge->next = fp;
    		new_merge = fp;
    		fp = fp->next;
    	} else {
    		new_merge->next = sp;
    		new_merge = sp;
    		sp = sp->next;
    	}
    }
    while (fp != nullptr) {
    	new_merge->next = fp;
    	new_merge = fp;
		fp = fp->next;
    }
    while (sp != nullptr) {
    	new_merge->next = sp;
    	new_merge = sp;
		sp = sp->next;
    }
    return new_head;
}

template <typename K>
Link<K>* create_linked_list(vector<K>& nums){
	if (nums.size() == 0) return nullptr;
	Link<int> *new_list = new Link<int>;
	*new_list = {nums[0], nullptr};
	Link<int> *head = new_list;
	for (int i = 1; i < nums.size(); ++i){
		Link<int> *new_elmt = new Link<int>; *new_elmt = {nums[i], nullptr};
	    new_list->next = new_elmt;
	    new_list = new_elmt;
	}
	return head;
}

template <typename K>
void print_list(Link<K>* head) {
	Link<K>* new_head = head;
	Link<K> *nxt = new_head->next;
	while (nxt) {
		cout << new_head->key << " ";
		nxt = nxt->next;
		new_head = new_head->next;
	}
	cout << new_head->key << " ";
	cout << endl;
}

int main() {
	vector<int> nums = {9,8,7,6,5,4,3,2,1};
	Link<int> *head = create_linked_list(nums);
	print_list(head);
	print_list(merge_sort(head));
	return 0;
}