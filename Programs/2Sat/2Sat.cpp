#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <utility>

using std::cout;
using std::cin;
using std::pair;
using std::vector;
using std::queue;
using std::stack;
using std::endl;

vector<int> adj[10001];
vector<int> adj_transpose[10001];
vector<int> adj_condensed[10001];
vector<int> components[10001];
bool visited[10001];
bool exists[10001];
stack<int> top_sort;
queue<int> reverse_top_sort;
int scc[10001];
int scc_count = 0;
short assignments[10001];

int negate(int var) {
	return 10001-var;
}

void DFS(int var, vector<int> G_adj[]) {
	if (visited[var]) return;
	visited[var] = true;
	for (auto v : G_adj[var]) {
		if (!visited[v]) {
			DFS(v, G_adj);
		}
	}
	top_sort.push(var);
}

void DFS_2(int var, vector<int> G_adj[]) {
	if (visited[var]) return;
	visited[var] = true;
	for (auto v : G_adj[var]) {
		if (!visited[v]) {
			DFS_2(v, G_adj);
		}
	}
	scc[var] = scc_count;
	components[scc_count].push_back(var);
}
void condense(int var, vector<int> G_adj[]) {
	if (visited[var]) return;
	visited[var] = true;
	for (auto v : G_adj[var]) {
		if (scc[var] == scc[v]) continue;
		else {
			vector<int>& n_adj = adj_condensed[scc[var]];
			if (std::find(n_adj.begin(), n_adj.end(), scc[v]) == n_adj.end()) n_adj.push_back(scc[v]);
		}
		if (!visited[v]) condense(v, G_adj);
	}
}

void DFS_last(int var, vector<int> G_adj[]) {
	if (visited[var]) return;
	visited[var] = true;
	for (auto v : G_adj[var]) {
		if (!visited[v]) {
			DFS_last(v, G_adj);
		}
	}
	reverse_top_sort.push(var);
}

int main() {
	int nv, nc;
	cin >> nv >> nc;
	// vector<pair<int,int>> test;
	for (int i = 0 ; i < nc; ++i) {
		int u_i, v_i; cin >> u_i >> v_i;
		// test.push_back({u_i,v_i});
		u_i = (u_i > 0) ? u_i : u_i+10001;
		v_i = (v_i > 0) ? v_i : v_i+10001;
		adj[negate(u_i)].push_back(v_i);
		adj_transpose[v_i].push_back(negate(u_i));
		adj[negate(v_i)].push_back(u_i);
		adj_transpose[u_i].push_back(negate(v_i));
		exists[u_i] = true;
		exists[negate(u_i)] = true;
		exists[v_i] = true;
		exists[negate(v_i)] = true;
	}

	for (int i = 1; i < 10001; ++i) if (exists[i] && !visited[i]) DFS(i,adj);
	for (int i = 1; i < 10001; ++i) visited[i] = false;
	while (!top_sort.empty()) {
		int t = top_sort.top(); top_sort.pop();
		if (!visited[t]) {
			DFS_2(t, adj_transpose);
			++scc_count;
		}
	}
	for (int i = 1; i <= 5000; ++i) {
		if (exists[i] && scc[i] == scc[negate(i)]) {
			cout << "Not satisfiable\n";
			return 0;
		}
	}
	for (int i = 1; i < 10001; ++i) visited[i] = false;
	for (int i = 1; i < 10001; ++i) if (!visited[i] && exists[i]) condense(i,adj);
	for (int i = 0; i < scc_count; ++i) visited[i] = false;
	for (int i = 0; i < scc_count; ++i) if (!visited[i]) DFS_last(i,adj_condensed);
	while (!reverse_top_sort.empty()){
		int t = reverse_top_sort.front(); reverse_top_sort.pop();
		for (auto v : components[t]) {
			if (assignments[v] == 0) {
				assignments[v] = 1;
				assignments[negate(v)] = -1;
			}
		}
	}
	// bool tests[10001];
	for (int i = 1; i <= nv; ++i) {
		// if (exists[i]) tests[i] = assignments[i] == 1 ? true : false;
		// else tests[i] = false;
		if (exists[i]) cout << (assignments[i] == 1 ? "T" : "F") << "\n";
		else cout << "T\n";
	}
	// for (auto x : test) {
	// 	int u=x.first; int v = x.second;
	// 	if (u>0&&v>0&&!(tests[u]||tests[v])) cout << "false\n";
	// 	if (u>0&&v<0&&!(tests[u]||!tests[-v])) cout << "false\n";
	// 	if (u<0&&v<0&&!(!tests[-u]||!tests[-v])) cout << "false\n";
	// 	if (u<0&&v>0&&!(!tests[-u]||tests[v])) cout << "false\n";
	// }

	return 0;
}