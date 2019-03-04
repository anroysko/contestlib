#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef vector<vector<int>> Graph;

// Struct for finding strongly connected components in a graph
// Condenced graph is in topological order
// "comp" gives component in condensed graph that node i is in
// Complexity: O(n + m log n)
struct SCC {
	Graph g; // Input graph
	Graph rev_g; // g reversed
	vector<int> topo; // Topological sort of g
	vector<int> comp; // Component of node i in res
	int cc = 0; // Component count

	void topoDfs(int i) {
		if (comp[i]) return;
		comp[i] = -1;
		for (auto t : g[i]) topoDfs(t);
		topo.push_back(i);
	}
	bool floodFill(int i) {
		if (comp[i] != -1) return false;
		comp[i] = cc;
		for (auto t : rev_g[i]) floodFill(t);
		return true;
	}
	SCC(const Graph& graph) : g(graph), comp(graph.size(), 0) {
		int n = g.size();
		rev_g.resize(n);
		for (int i = 0; i < n; ++i) {
			for (int t : g[i]) rev_g[t].push_back(i);
		}
		for (int i = 0; i < n; ++i) topoDfs(i);
		reverse(topo.begin(), topo.end());
		for (int i : topo) cc += floodFill(i);
	}
	Graph getCond() {
		Graph res(cc);
		for (int i = 0; i < g.size(); ++i) {	
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
		return res;
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
	}

	SCC scc(g);
	Graph res = scc.getCond();

	for (int i = 0; i < scc.cc; ++i) {
		cout << i+1 << ": ";
		for (auto t : res[i]) cout << t+1 << ' '; cout << '\n';
	}
	for (auto i : scc.comp) cout << i+1 << ' '; cout << '\n';
	for (auto i : scc.topo) cout << i+1 << ' '; cout << '\n';
}
