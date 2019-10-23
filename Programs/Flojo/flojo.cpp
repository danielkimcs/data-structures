#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

template<class T>
struct Link {
	explicit Link(const T& info, Link *next = 0) : info(info), next(next) { }
	// This avoids stack overflow
	~Link() {
		Link *p = next;
		while (p) {
			Link *q = p->next;
			p->next = 0;
			delete p;
			p = q;
		}
	}
	T info;
	Link *next;
};

vector<int> josephus(int n, int k) {
	Link<int> *head = new Link<int>(1);
	Link<int> *prev_link = head;
	for (int i = 2; i <= n; ++i) {
		Link<int> *new_link = new Link<int>(i);
		prev_link->next = new_link;
		prev_link = new_link;
	}
	prev_link->next = head;
	
	Link<int> *current = head;
	vector<int> deaths(n);
	int num_deaths = 0;
	int index = 0;
	while (current->next != current) {
		for (int i = 0; i < (k%(n-num_deaths)+(n-num_deaths))-2; ++i) {
			current = current->next;
		}
		Link<int> *del = current->next;
		deaths[index++] = del->info;
		++num_deaths;
		current->next = del->next;
		del->next = nullptr;
		delete del;
		current = current->next;
	}
	deaths[index] = current->info;
	current->next = nullptr;
	delete current;
	return deaths;
}

// template<class T>
vector<int> loopTail(Link<int>* head) {
	if (head) {
		if (head->next == nullptr) return {0,0};
		Link<int> *s = head, *f = head;
		s = s->next;
		f = (f->next)->next;
		while (f != nullptr && f != s) {
			s = s->next;
			if (f->next == nullptr) f = nullptr;
			else f = (f->next)->next;
		}
		vector<int> ans(2);
		int count = 0;
		if (f == nullptr) {
			ans[0] = 0;
		} else {
			++count;
			f = f->next;
			while (s != f) {
				f = f->next;
				++count;
			}
			ans[0] = count;
		}

		s = head, f = head;
		for (int i = 0; i < count; ++i) f = f->next;
		int tail_count = 0;
		while (f != nullptr) {
			if (s == f && count > 0) break;
			s = s->next;
			f = f->next;
			++tail_count;
		}
		ans[1] = tail_count;
		if (ans[0] == 0) ans[1]--;
		return ans;
	} else return {0,0};
}

void print_vec(vector<int> j) { // For testing
	for (int i = 0; i < j.size(); ++i) {
		cout << j[i] << " ";
	}
	cout << "\n";
}

int main() {
	// print_vec(josephus(20,20));

	// const int cycle_length = 4;
	// const int tail_length = 2;
	// Link<int>* head = new Link<int>(1);
	// Link<int>* o_head = head;
	// if (cycle_length == 1) {
	// 	head->next = head;
	// }
	// for (int i = 2;i <= tail_length+cycle_length;i++) {
	// 	head = new Link<int>(i,head);
	// 	if (i == cycle_length) {
	// 		o_head->next = head;
	// 	}
	// }
	Link<int> *head = new Link<int>(1);
	Link<int> *nxt = new Link<int>(2);
	head->next = nxt;
	// nxt->next = nullptr;
	// vector<int> ans = loopTail(head);
	print_vec(loopTail(head));
	// cout << "Cycle: " << ans[0] << " Tail: " << ans[1] << endl;
	// o_head->next = nullptr;
	nxt->next = nullptr;
	delete head;
	return 0;
}