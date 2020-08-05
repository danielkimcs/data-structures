#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <set>
#include <map>

using std::cout;
using std::string;
using std::cin;
using std::set;
using std::map;
using std::vector;
using std::endl;
using std::pair;

int N,D,W,H;
string rot_perms[23] = {"X", "Y", "XX", "XY", "YX", "YY", "XXX", "XXY", "XYX", "XYY", "YXX", "YYX", "YYY", "XXXY", "XXYX", "XXYY", "XYXX", "XYYY", "YXXX", "YYYX", "XXXYX", "XYXXX", "XYYYX"};
vector<vector<int>> axis_perms {{0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}};
vector<vector<int>> negative_perms {{0,0,0},{0,1,0},{0,1,1},{0,0,1},{1,0,0},{1,1,0},{1,1,1},{1,0,1}};

struct Link {
	Link* r;
	Link* l;
	Link* u;
	Link* d;
	Link* column;
	int row_id;
	int col_id;
	int size;
};

Link *header = new Link();

vector<vector<Link>> matrix;
vector<vector<bool>> cover_matrix;
int rows = 0, cols = 0;

vector<Link*> solutions;
vector<string> string_sols;

Link* construct_matrix() {
	for (int i = 0; i <= rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (cover_matrix[i][j]) {
				if (i) matrix[0][j].size += 1;
				matrix[i][j].column = &matrix[0][j];
				matrix[i][j].row_id = i;
				matrix[i][j].col_id = j;
				
				int a,b;
				a = i; b = j;
				do {b = (b+cols-1) % cols;} while (!cover_matrix[a][b] && b != j);
				matrix[i][j].l = &matrix[i][b];

				a = i; b = j;
				do {b = (b+1) % cols;} while (!cover_matrix[a][b] && b != j);
				matrix[i][j].r = &matrix[i][b];

				a = i; b = j;
				do {a = (a+rows) % (rows+1);} while (!cover_matrix[a][b] && a != i);
				matrix[i][j].u = &matrix[a][j];

				a = i; b = j;
				do {a = (a+1) % (rows+1);} while (!cover_matrix[a][b] && a != i);
				matrix[i][j].d = &matrix[a][j];
			}
		}
	}

	header->r = &matrix[0][0];
	header->l = &matrix[0][cols-1];
	matrix[0][0].l = header;
	matrix[0][cols-1].r = header;
	return header;
}

void cover(Link *target){
	Link *row, *rightNode;
	Link *colNode = target->column;
	colNode->l->r = colNode->r;
	colNode->r->l = colNode->l;
	for (row = colNode->d; row != colNode; row = row->d) {
		for (rightNode = row->r; rightNode != row; rightNode = rightNode->r) {
			rightNode->d->u = rightNode->u;
			rightNode->u->d = rightNode->d;
			matrix[0][rightNode->col_id].size -= 1;
		}
	}
}

void uncover(Link *target){
	Link *rowNode, *leftNode;
	Link *colNode = target->column;
	for (rowNode = colNode->u; rowNode != colNode; rowNode = rowNode->u) {
		for (leftNode = rowNode->l; leftNode != rowNode; leftNode = leftNode->l) {
			matrix[0][leftNode->col_id].size += 1;
			leftNode->d->u = leftNode;
			leftNode->u->d = leftNode;
		}
	}
	colNode->r->l = colNode;
	colNode->l->r = colNode;
}

Link* get_minimum_column() {
	Link *h = header;
	Link *mininum_column = h->r;
	h = h->r->r;
	do {
		if (h->size < mininum_column->size) mininum_column = h;
		h = h->r;
	} while (h != header);
	return mininum_column;
}

void record_solution() {
	vector<char> row_sol(D*W*H);
	for (auto x : solutions) {
		char current_piece;
		for (int i = 0; i < cols; ++i) {
			if (cover_matrix[x->row_id][i]){
				if (i < N) current_piece = 'A'+i;
				else {
					row_sol[i-N] = current_piece;
				}
			}
		}
	}
	string s(row_sol.begin(), row_sol.end());
	string_sols.push_back(s);
}

void search(int k) {
	Link *rowNode, *rightNode, *leftNode, *column;
	if (header->r == header) {
		record_solution();
		return;
	}
	column = get_minimum_column();
	cover(column);
	for (rowNode = column->d; rowNode != column; rowNode = rowNode->d) {
		solutions.push_back(rowNode);
		for (rightNode = rowNode->r; rightNode != rowNode; rightNode = rightNode->r) cover(rightNode);
		search(k+1);
		solutions.pop_back();
		column = rowNode->column;
		for (leftNode = rowNode->l; leftNode != rowNode; leftNode = leftNode->l) uncover(leftNode);
	}
	uncover(column);
}

struct Point {
	int x,y,z;
};

int encode(int x, int y, int z) {
	assert(y < D && x < W && z < H);
	return (W*H)*y+H*x+z;
}

vector<int> decode(int h){
	int y = h/(W*H);
	h %= (W*H);
	int x = h/H;
	h %= H;
	return {x,y,h};
}

void print_vector(vector<int>& a){
	for (auto n : a) {
		vector<int> h = decode(n);
		cout << "("<< h[0] << ", "<< h[1]<<", "<<h[2]<<")\n";
	}
	cout << endl;
}

vector<int> hash_polycube(vector<Point>& polycube) {
	vector<int> new_polycube;
	for (auto h : polycube) new_polycube.push_back(encode(h.x,h.y,h.z));
	sort(new_polycube.begin(), new_polycube.end());
	return new_polycube;
}

void translate_points(vector<Point>& polycube) {
	int min_x = 0, min_y = 0, min_z = 0;
	for (auto p : polycube) {
		if (p.x < min_x) min_x = p.x;
		if (p.y < min_y) min_y = p.y;
		if (p.z < min_z) min_z = p.z;
	}
	for (int i = 0; i < polycube.size(); ++i) {
		int new_x = polycube[i].x + abs(min_x);
		int new_y = polycube[i].y + abs(min_y);
		int new_z = polycube[i].z + abs(min_z);
		polycube[i] = {new_x, new_y, new_z};
		assert(new_x >= 0 && new_y >= 0 && new_z >= 0);
	}
}

void rotate_x(vector<Point>& a) {
	for (int i = 0; i < a.size(); ++i) a[i] = {a[i].x, (-1)*a[i].z, a[i].y};
}

void rotate_y(vector<Point>& a) {
	for (int i = 0; i < a.size(); ++i) a[i] = {a[i].z, a[i].y, (-1)*a[i].x};
}

vector<int> get_dimensions(vector<Point>& polycube) {
	int min_x = W, max_x = 0, min_y = D, max_y = 0, min_z = H, max_z = 0;
	for (auto l : polycube) {
		if (l.x < min_x) min_x = l.x;
		if (l.x > max_x) max_x = l.x;
		if (l.y < min_y) min_y = l.y;
		if (l.y > max_y) max_y = l.y;
		if (l.z < min_z) min_z = l.z;
		if (l.z > max_z) max_z = l.z;
	}
	return {max_x-min_x+1,max_y-min_y+1,max_z-min_z+1};
}

vector<int> get_dimensions(vector<int>& polycube) {
	int min_x = W, max_x = 0, min_y = D, max_y = 0, min_z = H, max_z = 0;
	for (auto l : polycube) {
		vector<int> coords = decode(l);
		if (coords[0] < min_x) min_x = coords[0];
		if (coords[0] > max_x) max_x = coords[0];
		if (coords[1] < min_y) min_y = coords[1];
		if (coords[1] > max_y) max_y = coords[1];
		if (coords[2] < min_z) min_z = coords[2];
		if (coords[2] > max_z) max_z = coords[2];
	}
	return {max_x-min_x+1,max_y-min_y+1,max_z-min_z+1};
}

bool check_dim(vector<Point>& polycube) {
	vector<int> initial_dim = get_dimensions(polycube);
	return (initial_dim[0] <= W && initial_dim[1] <= D && initial_dim[2] <= H);
}

vector<vector<int>> generate_all_rotations(vector<Point>& polycube) {
	translate_points(polycube);
	vector<vector<int>> rotations;
	if (check_dim(polycube)) rotations.push_back(hash_polycube(polycube));

	for (string r : rot_perms) {
		vector<Point> current(polycube);
		for (char c : r) {
			if (c == 'X') {
				rotate_x(current);
			}
			else {
				rotate_y(current);
			}
		}
		translate_points(current);
		bool dup = false;
		if (check_dim(current)) {
			vector<int> hashed_current = hash_polycube(current);
			for (auto k : rotations) {
				if (hashed_current == k) {
					dup = true;
					break;
				}
			}
			if (!dup) rotations.push_back(hashed_current);
		}
	}
	return rotations;
}

vector<vector<int>> generate_positions(vector<int>& polycube) {
	vector<int> dim = get_dimensions(polycube);
	vector<vector<int>> coords;
	for (int hash : polycube) {
		vector<int> c = decode(hash);
		coords.push_back(c);
	}
	vector<vector<int>> new_coords;
	for (int i = 0; i < W-dim[0]+1; ++i) {
		for (int j = 0; j < D-dim[1]+1; ++j) {
			for (int k = 0; k < H-dim[2]+1; ++k) {
				vector<int> translated_coords;
				for (auto p : coords) translated_coords.push_back(encode(p[0]+i,p[1]+j,p[2]+k));
				new_coords.push_back(translated_coords);
			}
		}
	}

	return new_coords;
}

string sol_hash(string solution) {
	map<char, char> char_map;
	vector<char> new_string;
	char latest_char = 'A';
	for (char c : solution) {
		if (char_map.find(c) == char_map.end()) {
			char_map[c] = latest_char;
			latest_char = static_cast<char>(latest_char + 1);
		}
		new_string.push_back(char_map[c]);
	}
	string new_sol(new_string.begin(), new_string.end());
	return new_sol;
}

vector<string> prune_solutions() {
	set<string> hashed_sols;
	vector<string> distinct_sols;
	for (string sol : string_sols) {
		if (hashed_sols.count(sol_hash(sol)) == 0) {
			hashed_sols.insert(sol_hash(sol));
			distinct_sols.push_back(sol);
		}
		vector<pair<Point, char>> converted;
		for (int i = 0; i < sol.length(); ++i) {
			vector<int> coords = decode(i);
			converted.push_back({{coords[0], coords[1], coords[2]}, sol[i]});
		}
		for (auto a : axis_perms) {
			for (auto b : negative_perms) {
				vector<pair<Point, char>> oriented_sol(converted);
				vector<char> oriented_sol_chars;
				int min_x = 0, min_y = 0, min_z = 0;
				for (int i = 0; i < oriented_sol.size(); ++i) {
					pair<Point, char> p = oriented_sol[i];
					vector<int> coords {p.first.x, p.first.y, p.first.z};
					Point new_point = {coords[a[0]], coords[a[1]], coords[a[2]]};
					new_point.x *= (b[0] == 0 ? 1 : -1);
					if (new_point.x < min_x) min_x = new_point.x;
					new_point.y *= (b[1] == 0 ? 1 : -1);
					if (new_point.y < min_y) min_y = new_point.y;
					new_point.z *= (b[2] == 0 ? 1 : -1);
					if (new_point.z < min_z) min_z = new_point.z;
					oriented_sol[i] = {new_point, p.second};
				}
				bool bad_rot = false;
				for (int i = 0; i < oriented_sol.size(); ++i) {
					pair<Point, char> p = oriented_sol[i];
					p = {{p.first.x - min_x, p.first.y - min_y, p.first.z - min_z}, p.second};
					if (p.first.x >= W || p.first.y >= D || p.first.z >= H) {
						bad_rot = true;
						break;
					}
				}
				if (bad_rot) continue;
				sort(oriented_sol.begin(), oriented_sol.end(), []
					(const pair<Point, char> &a, const pair<Point, char> &b) {
						return encode(a.first.x, a.first.y, a.first.z) < encode(b.first.x, b.first.y, b.first.z);
					}
				);
				for (auto p : oriented_sol) oriented_sol_chars.push_back(p.second);
				string oriented_sol_str(oriented_sol_chars.begin(), oriented_sol_chars.end());
				hashed_sols.insert(sol_hash(oriented_sol_str));
			}
		}
	}
	return distinct_sols;
}

int main() {
	cin >> N;
	string pieces_input[N];
	for (int i = 0; i < N; ++i) cin >> pieces_input[i];
	cin >> D >> W >> H;
	vector<Point> pieces[N];
	for (int i = 0; i < N; ++i) {
		vector<Point> piece;
		vector<Point> traversed;
		piece.push_back({0,0,0});
		traversed.push_back({0,0,0});
		for (char c : pieces_input[i]) {
			if (isdigit(c)) {
				int moves_back = c-'0';
				for (int i = 0; i < moves_back; ++i) traversed.pop_back();
			} else {
				Point n;
				if (c == 'B') n = {traversed[traversed.size()-1].x, traversed[traversed.size()-1].y-1, traversed[traversed.size()-1].z};
				else if (c == 'F') n = {traversed[traversed.size()-1].x, traversed[traversed.size()-1].y+1, traversed[traversed.size()-1].z};
				else if (c == 'N') n = {traversed[traversed.size()-1].x, traversed[traversed.size()-1].y, traversed[traversed.size()-1].z+1};
				else if (c == 'S') n = {traversed[traversed.size()-1].x, traversed[traversed.size()-1].y, traversed[traversed.size()-1].z-1};
				else if (c == 'E') n = {traversed[traversed.size()-1].x+1, traversed[traversed.size()-1].y, traversed[traversed.size()-1].z};
				else if (c == 'W') n = {traversed[traversed.size()-1].x-1, traversed[traversed.size()-1].y, traversed[traversed.size()-1].z};
				piece.push_back(n);
				traversed.push_back(n);
			}
		}
		traversed.clear();
		pieces[i] = piece;
	}
	cols = N+D*W*H;
	vector<bool> header_row(cols,true);
	cover_matrix.push_back(header_row);
	vector<int> piece_count(N,0);
	for (int i = 0; i < N; ++i) {
		for (auto v : generate_all_rotations(pieces[i])) {
			for (auto position : generate_positions(v)) {
				vector<bool> new_row(cols,false);
				new_row[i] = true;
				for (auto p : position) new_row[N+p] = true;
				cover_matrix.push_back(new_row);
				piece_count[i]++;
			}
		}
	}
	rows = cover_matrix.size()-1;
    matrix.resize(rows+1);
    for (int i = 0; i < rows+1; ++i) matrix[i].resize(cols+1);
    
    construct_matrix();
    search(0);

    vector<string> pruned = prune_solutions();
    cout << pruned.size()<<endl;
    for (auto x : pruned) cout << x << endl;
   
    return 0;
}