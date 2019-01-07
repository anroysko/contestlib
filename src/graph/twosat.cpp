#include <iostream>
#include <vector>
using namespace std;
typedef vector<vector<int>> Graph;

// Struct for finding strongly connected components in a graph
// Condensed graph is in reverse topological order
// "comp" gives component in condensed graph that node i is in
// Time Complexity: O(n + m)
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
};

// Struct for solving twosat
// addClause(i, twosat.neg(j)) adds clause i, -j
// solve() returns empty vector if unsolvable
// Time Complexity: O(n + m)
struct TwoSat {
	const int n;
	Graph g; // Implication graph, of size 2*n

	TwoSat(int nodes) : n(nodes), g(2*nodes) {}

	int neg(int i) {
		return i >= n ? i-n : i+n;
	}
	void addClause(int a, int b) {
		g[neg(a)].push_back(b);
		g[neg(b)].push_back(a);
	}
	vector<bool> solve() {
		SCC scc(g);

		vector<int> inv(scc.cc);
		for (int i = 0; i < 2*n; ++i) {
			inv[scc.comp[i]] = scc.comp[neg(i)];
		}

		vector<int> state(scc.cc, -1);
		for (int i = 0; i < scc.cc; ++i) {
			if (state[i] != -1) continue;
			if (i == inv[i]) return {};
			state[i] = 1;
			state[inv[i]] = 0;
		}

		vector<bool> res(n);
		for (int i = 0; i < n; ++i) res[i] = state[scc.comp[i]];
		return res;
	}
};

// Example usage
int main() {
	int m, n;
	cin >> m >> n;
	TwoSat twosat(n);
	for (int j = 0; j < m; ++j) {
		int a, b;
		cin >> a >> b;
		if (a < 0) a = twosat.neg(-a-1);
		else a = a-1;
		if (b < 0) b = twosat.neg(-b-1);
		else b = b-1;
		twosat.addClause(a, b);
	}
	vector<bool> res = twosat.solve();
	if (res.empty()) {
		cout << "NO\n";
	} else {
		cout << "YES\n";
		for (auto it : res) cout << it; cout << '\n';
	}
}
