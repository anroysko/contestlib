#include <iostream>
#include <vector>
using namespace std;
typedef vector<vector<int>> Graph;

// Struct for finding strongly connected components in a graph
// Condensed graph is in reverse topological order
// "comp" gives component in condensed graph that node i is in
// Complexity: O(n + m)
struct SCC {
	Graph g;
	vector<int> comp;
	vector<int> ind;
	vector<int> sta;
	int di = 0; // DFS counter
	int cc = 0; // Comp count

	int dfs(int i) {
		if (ind[i] != -1) return (comp[i] == -1) ? ind[i] : di;
		ind[i] = di;
		int md = di;
		++di;

		sta.push_back(i);
		for (auto t : g[i]) md = min(md, dfs(t));

		if (md == ind[i]) {
			while(comp[i] == -1) {
				comp[sta.back()] = cc;
				sta.pop_back();
			}
			++cc;
		}
		return md;
	}
	SCC(const Graph& gr) : g(gr), comp(gr.size(), -1), ind(gr.size(), -1) {
		for (int i = 0; i < g.size(); ++i) dfs(i);
	}
	Graph getCond() {
		Graph res(cc);
		for (int i = 0; i < g.size(); ++i) {
			for (auto t : g[i]) res[comp[i]].push_back(comp[t]);
		}
		vector<bool> seen(cc, false);
		for (int i = 0; i < cc; ++i) {
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
	for (int i = 0; i < res.size(); ++i) {
		cout << i+1 << ": ";
		for (auto t : res[i]) cout << t+1 << ' ' ; cout << '\n';
	}
	for (int i : scc.comp) cout << i+1 << ' '; cout << '\n';
}