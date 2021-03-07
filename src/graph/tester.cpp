#include <bits/stdc++.h>
using namespace std;
using ll = long long;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

const int H = 1e9;
const int N = 2000;
const int M = 30000;

void dfs(int i, vector<bool>& vis, const vector<vector<int>>& g) {
	if (vis[i]) return;
	vis[i] = 1;
	for (auto t : g[i]) dfs(t, vis, g);
}

void genTest(ofstream& fout) {
	while(true) {
		int n = rand(1, N);
		vector<pair<int, int>> ed;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (i != j) ed.emplace_back(i, j);
			}
		}
		shuffle(ed.begin(), ed.end(), rng);
		int m = rand(0, (int)ed.size());
		int r = rand(0, n-1);

		vector<bool> vis(n, 0);
		vector<vector<int>> g(n);
		for (int i = 0; i < m; ++i) g[ed[i].first].push_back(ed[i].second);
		dfs(r, vis, g);
		bool works = 1;
		for (auto v : vis) works &= v;
		if (! works) continue;

		fout << n << ' ' << m << ' ' << r << '\n';
		for (int j = 0; j < m; ++j) fout << ed[j].first << ' ' << ed[j].second << ' ' << rand(0, H) << '\n';
		break;
	}
}

int main() {
	for (int ti = 0;; ++ti) {
		cerr << ti << ' ';
		if (ti % 10 == 9) cerr << endl;

		ofstream fout;
		fout.open("test.in");
		
		/*
		int t = TC;
		fout << t << '\n';
		for (int ti = 0; ti < t; ++ti) genTest(fout);
		*/

		genTest(fout);
		fout.flush();
		fout.close();

		break;

		bool fail = false;
		fail |= system("./dir_mst < test.in > code.out");
		fail |= system("./comp < test.in > brute.out");
		fail |= system("diff code.out brute.out");
		if (fail) {
			cout << "BUG FOUND!\n";
			break;
		}
	}
}
