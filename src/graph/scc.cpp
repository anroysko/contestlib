#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Graph {
	vector<vector<int>> conns;

	void init(int n) {
		conns.resize(n);
	}
	vector<int>& operator[](int i) {
		return conns[i];
	}
	int size() const {
		return conns.size();
	}
};

// Struct for finding strongly connected components in a graph
// Complexity: O(n + m log n)
struct SCC {
	Graph g; // Input graph
	Graph rev_g; // g reversed
	vector<int> topo; // Topological sort of g
	vector<int> comp; // Component of node i in res
	Graph res; // result graph

	void topoDfs(int i) {
		if (comp[i]) return;
		comp[i] = -1;
		for (auto t : g[i]) topoDfs(t);
		topo.push_back(i);
	}
	bool floodFill(int i, int v) {
		if (comp[i] != -1) return false;
		comp[i] = v;
		for (auto t : rev_g[i]) floodFill(t, v);
		return true;
	}
	void init(const Graph& graph) {
		int n = graph.size();
		g = graph;

		// Build reverse graph
		rev_g.init(n);
		for (int i = 0; i < n; ++i) {
			for (int t : g[i]) rev_g[t].push_back(i);
		}

		// Find topo order
		comp.resize(n, 0);
		for (int i = 0; i < n; ++i) topoDfs(i);
		reverse(topo.begin(), topo.end());

		// Find comps
		int cc = 0;
		for (int i : topo) cc += floodFill(i, cc);

		// Build res graph
		res.init(cc);
		for (int i = 0; i < n; ++i) {	
			for (auto t : g[i]) {
				int ic = comp[i];
				int tc = comp[t];
				if (ic != tc) res[ic].push_back(tc);
			}
		}
		for (int i = 0; i < cc; ++i) {
			sort(res[i].begin(), res[i].end());
			res[i].erase(unique(res[i].begin(), res[i].end()), res[i].end());
		}
	}
};

// Example usage
int main() {
	int n, m;
	cin >> n >> m;
	Graph g; g.init(n);
	for (int j = 0; j < m; ++j) {
		int a, b;
		cin >> a >> b;
		--a; --b;
		g[a].push_back(b);
	}

	SCC scc; scc.init(g);

	for (int i = 0; i < scc.res.size(); ++i) {
		for (auto t : scc.res[i]) cout << i+1 << " -> " << t+1 << '\n';
	}
	for (int i = 0; i < n; ++i) cout << scc.comp[i]+1 << ' '; cout << '\n';
	for (int i = 0; i < n; ++i) cout << scc.topo[i]+1 << ' '; cout << '\n';
}
