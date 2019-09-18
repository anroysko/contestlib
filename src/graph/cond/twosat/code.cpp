
using Graph = vector<vector<int>>;

// Struct for finding strongly connected components in a graph
// Condensed graph is in reverse topological order (node with no out-edges first)
// Complexity: O(n + m)
class SCC {
	private:
		Graph g;
		vector<int> comp, ind, sta;
		int di = 0, cc = 0;

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
	public:
		SCC(const Graph& gr) : g(gr), comp(gr.size(), -1), ind(gr.size(), -1) {
			for (int i = 0; i < g.size(); ++i) dfs(i);
		}
		int compCount() const { return cc; }
		int operator[](int i) const { return comp[i]; }
};


// Struct for solving twosat.
// [0, n) are variables, [n, 2n) negations. neg(i) negates a variable.
// addClause(i, twosat.neg(j)) adds clause i, -j
// solve() returns empty vector if unsolvable
// Time Complexity: O(n + m)
class TwoSat {
	private:
		const int n;
		Graph g; // Implication graph, of size 2*n
	public:
		TwoSat(int nodes) : n(nodes), g(2*nodes) {}
		int neg(int i) { return i >= n ? i-n : i+n; }

		void addClause(int a, int b) {
			g[neg(a)].push_back(b);
			g[neg(b)].push_back(a);
		}
		vector<bool> solve() {
			SCC scc(g);
			int m = scc.compCount();

			vector<int> inv(m);
			for (int i = 0; i < 2*n; ++i) inv[scc[i]] = scc[neg[i]];

			vector<int> state(m, -1);
			for (int i = 0; i < m; ++i) {
				if (state[i] != -1) continue;
				if (i == inv[i]) return {};
				state[i] = 1;
				state[inv[i]] = 0;
			}

			vector<bool> res(n);
			for (int i = 0; i < n; ++i) res[i] = state[scc[i]];
			return res;
		}
};
