#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

struct Graph {
	vector<vector<int>> conns;

	Graph(int n) : conns(n) {}

	vector<int>& operator[](int i) {
		return conns[i];
	}
	const vector<int>& operator[](int i) const {
		return conns[i];
	}
	int size() const {
		return conns.size();
	}
};

void topoDfs(const Graph& g, vector<bool>& vis, vector<int>& topo, int i) {
	if (vis[i]) return;
	vis[i] = true;
	for (auto t : g[i]) topoDfs(g, vis, topo, t);
	topo.push_back(i);
}

// Gets a topological order of the given graph
// nodes in same strongly connected component may be in any order
vector<int> topoSort(const Graph& g) {
	vector<bool> vis(g.size(), false);
	vector<int> topo;
	for (int i = 0; i < g.size(); ++i) {
		topoDfs(g, vis, topo, i);
	}
	reverse(topo.begin(), topo.end());
	return topo;
}

// Returns graph with direction of all edges reversed, compared to g
Graph reverseGraph(const Graph& g) {
	Graph rev(g.size());
	for (int i = 0; i < g.size(); ++i) {
		for (auto t : g[i]) rev[t].push_back(i);
	}
	return rev;
}

bool floodFill(const Graph& g, vector<int>& comp, int i, int v) {
	if (comp[i] != -1) return false;
	comp[i] = v;
	for (int t : g[i]) floodFill(g, comp, t, v);
	return true;
}

// Finds strongly connected components in a graph
// Returns {r, v}, where r is a graph with merged
// connected components, and v tells which comp in r
// nodes in g belong to.
// Time Complexity: O(n + m log n)
pair<Graph, vector<int>> connComp(const Graph& g) {
	Graph gr = reverseGraph(g);
	vector<int> topo = topoSort(g);

	int cc = 0; // Comp count
	vector<int> comp(g.size(), -1);
	for (int i : topo) {
		cc += floodFill(gr, comp, i, cc);
	}

	Graph res(cc);
	for (int i : topo) {
		int ic = comp[i];
		for (auto t : g[i]) {
			int tc = comp[t];
			if (ic != tc) res[ic].push_back(tc);
		}
	}
	for (int i = 0; i < cc; ++i) {
		sort(res[i].begin(), res[i].end());
		res[i].erase(unique(res[i].begin(), res[i].end()), res[i].end());
	}
	return {res, comp};
}

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

	auto scc = connComp(g);
	for (int i = 0; i < scc.first.size(); ++i) {
		for (auto t : scc.first[i]) cout << i+1 << " -> " << t+1 << '\n';
	}
	for (int i = 0; i < n; ++i) cout << scc.second[i]+1 << ' '; cout << '\n';
}
