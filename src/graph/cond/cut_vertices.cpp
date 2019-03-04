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

// Struct for finding cut vertices in an undirected graph.
// Complexity: O(n + m)
struct CutVertices {
	Graph g;
	vector<int> comp;
	vector<int> ind;
	vector<bool> cutv;
	int di = 0; // DFS counter
	int cc = 0; // Comp count

	int dfs(int i) {
		ind[i] = di;
		int res = di;
		int outs = 0;
		++di;

		for (auto t : g[i]) {
			if (ind[t] != -1) res = min(res, ind[t]);
			else {
				int sub = dfs(t);
				if (sub >= ind[i]) cutv[i] = true;
				else res = min(res, sub);
				++outs;
			}
		}
		if (res == ind[i] && outs < 2) cutv[i] = false;
		return res;
	}
	bool flood(int i) {
		if (comp[i] != -1) return false;
		comp[i] = cc;
		for (auto t : g[i]) {
			if ((! cutv[i]) && (! cutv[t])) flood(t);
		}
		return true;
	}
	CutVertices(const Graph& gr) : g(gr), comp(gr.size(), -1), ind(gr.size(), -1), cutv(gr.size(), 0) {
		for (int i = 0; i < g.size(); ++i) {
			if (ind[i] == -1) dfs(i);
		}
		for (int i = 0; i < g.size(); ++i) cc += flood(i);
	}
};

// Example usage
int main() {
	int n, m;
	cin >> n >> m;

	Graph g(n);
	for (int j = 0; j < m; ++j) {
		int a, b;
		cin >> a >> b;
		--a; --b;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	CutVertices cv(g);

	// Print cut vertices
	for (int i = 0; i < n; ++i) {
		if (cv.cutv[i]) cout << i+1 << ' ';
	}
	cout << '\n';
	
	// Print condensation
	Graph res = getCond<CutVertices>(cv);
	for (int i = 0; i < res.size(); ++i) {
		cout << i+1 << ": ";
		for (auto t : res[i]) cout << t+1 << ' ' ; cout << '\n';
	}
	for (auto i : cv.comp) cout << i+1 << ' '; cout << '\n';
}
