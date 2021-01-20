#include <bits/stdc++.h>

using namespace std;

namespace utils {
	
	vector<vector<int>> goal = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
	
	int fast_pow(int base, int exp) {
		if(!exp)
			return 1;
		if(exp & 1)
			return base * fast_pow(base, exp - 1);
		int temp = fast_pow(base, exp / 2);
		return temp * temp;
	}
	
	bool is_valid(pair<int,int> &p) {
		if(p.first < 0 || p.first > 2 || p.second < 0 || p.second > 2)
			return 0;
		return 1;
	}
	
	void print_int_puzzle(int mask) {
		stack<int> digs;
		while(mask) {
			digs.push(mask % 10);
			mask /= 10;
		}
		if(digs.size() == 8)
			digs.push(0);
		for(int i = 0 ; i < 3 ; i++) {
			cout<<"    *  ";
			for(int j = 0 ; j < 3 ; j++) {
				cout<<digs.top()<<" ";
				digs.pop();
			}
			cout<<" *"<<endl;
		}
	}
	
	void print_path(unordered_map<int,int> &par) {
		int curr = 12345678;
		stack<int> path;
		path.push(curr);
		while(par.find(curr) != par.end()) {
			curr = par[curr];
			path.push(curr);
		}
		cout<<"Cost of this path = "<<path.size() - 1<<endl<<endl;
		cout<<"    ***********"<<endl;
		while(path.size()) {
			utils::print_int_puzzle(path.top());
			path.pop();
			cout<<"    ***********"<<endl;
		}
		cout<<endl<<endl;
		cout<<"    ___________    "<<endl<<endl;
	}
	
	bool is_goal(int state) {
		return (state == 12345678);
	}
	
	int count_entries(unordered_map<int,int> &m) {
		int cnt = 0;
		for(auto it : m)
			if(it.second) // To remove warning.
				cnt++;
		return cnt;
	}
};

class Puzzle{
public:
	Puzzle() {
		this->puzzle = vector<vector<int>>(3, vector<int>(3));
	}
	
	Puzzle(vector<vector<int>> puzzle) : puzzle(puzzle) {}
	
	Puzzle(int mask) {
		this->puzzle = vector<vector<int>>(3, vector<int>(3));
		stack<int> digs;
		while(mask) {
			digs.push(mask % 10);
			mask /= 10;
		}
		if(digs.size() == 8)
			digs.push(0);
		for(int i = 0 ; i < 3 ; i++) {
			for(int j = 0 ; j < 3 ; j++) {
				this->puzzle[i][j] = digs.top();
				digs.pop();
			}
		}
	}

	vector<vector<int>> get_puzzle() {
		return this->puzzle;
	}
	
	void read_puzzle() {
		for(int i = 0 ; i < 3 ; i++)
			for(int j = 0 ; j < 3 ; j++)
				cin>>this->puzzle[i][j];
	}
	
	int to_int() {
		// Will be in format [0][0] * 10^9 + [0][1] * 10^8 + ...
		int mask = 0;
		int curr_power = 8;
		for(int i = 0 ; i < 3 ; i++) {
			for(int j = 0 ; j < 3 ; j++) {
				mask += utils::fast_pow(10, curr_power--) * this->puzzle[i][j];
			}
		}
		return mask;
	}
	
	void swap(pair<int,int> p1, pair<int,int> p2) {
		int val = this->puzzle[p1.first][p1.second];
		this->puzzle[p1.first][p1.second] = this->puzzle[p2.first][p2.second];
		this->puzzle[p2.first][p2.second] = val;
	}
	
	vector<int> get_childs() {
		vector<int> valid_childs;
		pair<int,int> zero = get_zero();
		for(int i = 0 ; i < 4 ; i++) {
			pair<int,int> new_pos = {zero.first + dr[i], zero.second + dc[i]};
			if(utils::is_valid(new_pos)) {
				Puzzle *new_p = new Puzzle(this->puzzle);
				new_p->swap(zero, new_pos);
				valid_childs.push_back(new_p->to_int());
			}
		}
		return valid_childs;
	}
	
	void print() {
		for(int i = 0 ; i < 3 ; i++) {
			for(int j = 0 ; j < 3 ; j++)
				cout<<this->puzzle[i][j]<<" ";
			cout<<endl;
		}
	}
	
	int get_heuristic_manhattan(){
		int ans = 0;
		map<int, pair<int,int>> positions;
		for(int i = 0 ; i < 3 ; i++)
			for(int j = 0 ; j < 3 ; j++)
				positions[utils::goal[i][j]] = {i, j};
		for(int i = 0 ; i < 3 ; i++) {
			for(int j = 0 ; j < 3 ; j++) {
				int curr_num = this->puzzle[i][j];
				pair<int,int> pos = positions[curr_num];
				ans = ans + abs(i - pos.first) + abs(j - pos.second);
			}
		}
		return ans;
	}
	
	int get_heuristic_euclidean(){
		int ans = 0;
		map<int, pair<int,int>> positions;
		for(int i = 0 ; i < 3 ; i++)
			for(int j = 0 ; j < 3 ; j++)
				positions[utils::goal[i][j]] = {i, j};
		for(int i = 0 ; i < 3 ; i++) {
			for(int j = 0 ; j < 3 ; j++) {
				int curr_num = this->puzzle[i][j];
				pair<int,int> pos = positions[curr_num];
				ans = ans + utils::fast_pow(i - pos.first, 2) + utils::fast_pow(j - pos.second, 2);
			}
		}
		return ans;
	}

		
private:
	vector<vector<int>> puzzle;
	int dr[4] = {1, -1, 0 ,0};
	int dc[4] = {0, 0, 1, -1};
	
	pair<int,int> get_zero() {
		for(int i = 0 ; i < 3 ; i++)
			for(int j = 0 ; j < 3 ; j++)
				if(this->puzzle[i][j] == 0)
					return {i, j};
		return {-1, -1}; // Wrong puzzle.
	}
};

class DFS {
public:
	DFS(int state) : state(state) {} // check that solved starts with 0.

	void start() {
		const clock_t begin_time = clock();
		solve(state);
		const clock_t end_time = clock();
		int nodes_vis = utils::count_entries(vis);
		cout<<"Number of visited nodes in DFS : "<<nodes_vis<<endl;
		cout<<"Maximum depth in DFS : "<<max_depth<<endl;
		cout<<"Time taken by DFS : "<<float(end_time - begin_time) * 1000 / CLOCKS_PER_SEC<<" ms"<<endl; 
		print_path();
	}
	
	void solve(int state, int depth = 0) {
		if(solved || utils::is_goal(state)) {
			solved = 1;
			if(utils::is_goal(state))
				max_depth = max(max_depth, depth), vis[state] = 1; // To mark goal as visited
			return;
		}
		max_depth = max(max_depth, depth);
		vis[state] = 1;
		Puzzle *p = new Puzzle(state);
		vector<int> childs = p->get_childs();
		for(int i = 0 ; i < (int)childs.size() ; i++) {
			if(!vis[childs[i]]) {
				par[childs[i]] = state;
				solve(childs[i], depth + 1);
				//vis[childs[i]] = 0;
			}
			if(solved)
				return;
		}
	}
	
	void print_path() {
		utils::print_path(par);
	}
	
private:
	unordered_map<int,int> vis, par;
	int state, solved, max_depth;
};

class BFS {
public:	
	BFS(int state) : state(state) {}
	
	void start() {
		const clock_t begin_time = clock();
		solve(state);
		const clock_t end_time = clock();
		int nodes_vis = utils::count_entries(vis);
		cout<<"Number of visited nodes in BFS : "<<nodes_vis<<endl;
		cout<<"Maximum depth in BFS : "<<max_depth<<endl;
		cout<<"Time taken by BFS : "<<float(end_time - begin_time) * 1000 / CLOCKS_PER_SEC<<" ms"<<endl;
		print_path();
	}
	
	void solve(int state) {
		if(utils::is_goal(state)) {
			solved = 1;
			return;
		}
		queue<int> q;
		q.push(state);
		vis[state] = 1;
		while(q.size()) {
			int curr = q.front();
			q.pop();
			Puzzle *p = new Puzzle(curr);
			vector<int> childs = p->get_childs();
			for(int i = 0 ; i < (int)childs.size() ; i++) {
				if(!vis[childs[i]]) {
					vis[childs[i]] = vis[curr] + 1;
					max_depth = max(max_depth, vis[childs[i]] - 1);
					par[childs[i]] = curr;
					if(utils::is_goal(childs[i])) {
						solved = 1;
						return;
					}
					q.push(childs[i]);
				}
			}
		}
		return;
	}
	
	void print_path() {
		utils::print_path(par);
	}
	
private:
	unordered_map<int,int> vis, par;
	int state, solved, max_depth;
};

class A_Star_Search {
public:
	A_Star_Search(int state) : state(state) {}
	
	enum {MANHATTAN, EUCLIDEAN};
	
	void start_manhattan() {
		const clock_t begin_time = clock();
		solve(state, 0, MANHATTAN);
		const clock_t end_time = clock();
		int nodes_vis = utils::count_entries(vis);
		cout<<"Number of visited nodes in A* (Manhattan heuristic): "<<nodes_vis<<endl;
		cout<<"Maximum depth in A* (Manhattan heuristic) : "<<max_depth<<endl;
		cout<<"Time taken by A* (Manhattan heuristic) : "<<float(end_time - begin_time) * 1000 / CLOCKS_PER_SEC<<" ms"<<endl;
		print_path();
	}
	
	void start_euclidean() {
		const clock_t begin_time = clock();
		solve(state, 0, EUCLIDEAN);
		const clock_t end_time = clock();
		int nodes_vis = utils::count_entries(vis);
		cout<<"Number of visited nodes in A* (Euclidean heuristic) : "<<nodes_vis<<endl;
		cout<<"Maximum depth in A* (Euclidean heuristic) : "<<max_depth<<endl;
		cout<<"Time taken by A* (Euclidean heuristic) : "<<float(end_time - begin_time) * 1000 / CLOCKS_PER_SEC<<" ms"<<endl;
		print_path();
	}
	
	void solve(int state, int depth, int type) {
		if(solved || utils::is_goal(state)) {
			solved = 1;
			if(utils::is_goal(state))
				max_depth = max(max_depth, depth), vis[state] = 1;
			return;
		}
		max_depth = max(max_depth, depth);
		vis[state] = 1;
		Puzzle *p = new Puzzle(state);
		set<pair<int,int>> sorted_childs;
		vector<int> childs = p->get_childs();
		for(int i = 0 ; i < (int)childs.size() ; i++) {
			if(!vis[childs[i]]) {
				Puzzle *child = new Puzzle(childs[i]);
				int cost = depth + 1;
				if(type == MANHATTAN)
					cost += child->get_heuristic_manhattan();
				if(type == EUCLIDEAN)
					cost += child->get_heuristic_euclidean();
				par[childs[i]] = state;
				sorted_childs.insert({cost, childs[i]});
			}
		}
		for(auto it : sorted_childs) {
			solve(it.second, depth + 1, type);
		}
	}
	
	void print_path() {
		utils::print_path(par);
	}
	
private:
	unordered_map<int,int> par, vis;
	int state, solved, max_depth;
	
};

int main(){
	freopen("test2.txt", "r", stdin);
	Puzzle *p = new Puzzle();
	p->read_puzzle();
	DFS *dfs = new DFS(p->to_int());
	BFS *bfs = new BFS(p->to_int());
	A_Star_Search *as_man = new A_Star_Search(p->to_int());
	A_Star_Search *as_euc = new A_Star_Search(p->to_int());
	dfs->start();
	bfs->start();
	as_man->start_manhattan();
	as_euc->start_euclidean();
	return 0;
}
