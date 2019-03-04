#include <iostream>
#include <vector>
using namespace std;
typedef vector<vector<int>> Graph;

// Gets condensed version of a graph.
// Requires .cc (comp count), .g (orignal graph), .comp (comp in condensed graph)
// Complexity: O(n + m)
template<class T>
Graph getCond(const T& s) {
	Graph res(s.cc);
	for (int i = 0; i < s.g.size(); ++i) {
		for (auto t : s.g[i]) res[s.comp[i]].push_back(s.comp[t]);
	}

	// Remove self-loops and duplicate edges
	vector<bool> seen(s.cc, false);
	for (int i = 0; i < s.cc; ++i) {
		seen[i] = true; // Remove self-loop
		int j = 0; // Number of actuals
		int r = 0; // Number of removes
		while(j+r < res[i].size()) {
			swap(res[i][j], res[i][j+r]);
			if (seen[res[i][j]]) ++r;
			else {
				seen[res[i][j]] = true;
				++j;
			}
		}
		res[i].resize(j);
		for (int v : res[i]) seen[v] = false;
		seen[i] = false;
	}
	return res;
}

// Struct for finding bridges in an undirected graph.
// Complexity: O(n + m)
struct Bridges {
	Graph g;
	vector<int> comp;
	vector<int> ind;
	int di = 0; // DFS counter
	int cc = 0; // Comp count

	int dfs(int i, int p) {
		ind[i] = di;
		int res = di;
		++di;

		for (auto t : g[i]) {
			if (t == p) p = -1; // Multi-edge to parent cannot be bridge
			else if (ind[t] != -1) res = min(res, ind[t]);
			else {
				int sub = dfs(t, i);
				res = min(res, sub);
				if (sub > i) ++cc; // Bridge
			}
		}

		comp[i] = cc - 1;
		return res;
	}
	Bridges(const Graph& gr) : g(gr), comp(gr.size(), -1), ind(gr.size(), -1) {
		for (int i = 0; i < g.size(); ++i) {
			if (ind[i] != -1) continue;
			++cc;
			dfs(i, i);
		}
	}
	bool isBridge(int a, int b) {
		return comp[a] != comp[b];
	}
};

// Example usage
int main() {
	int n, m;
	cin >> n >> m;

	Graph g(n);
	vector<pair<int, int>> inds(m);
	for (int j = 0; j < m; ++j) {
		int a, b;
		cin >> a >> b;
		--a; --b;
		g[a].push_back(b);
		g[b].push_back(a);
		inds[j] = {a, b};
	}

	Bridges brd(g);

	// Print bridges
	for (int i = 0; i < m; ++i) {
		if (brd.isBridge(inds[i].first, inds[i].second)) cout << i+1 << '\n';
	}
	cout << '\n';
	
	// Print condensation
	Graph res = getCond<Bridges>(brd);
	for (int i = 0; i < res.size(); ++i) {
		cout << i+1 << ": ";
		for (auto t : res[i]) cout << t+1 << ' ' ; cout << '\n';
	}
	for (auto i : brd.comp) cout << i+1 << ' '; cout << '\n';
}
