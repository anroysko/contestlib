using Graph = vector<vector<int>>;

// Condenses components of nodes into single nodes. There is an edge from
// component X to Y if there's an edge from some node in X to some node in Y.
// Time complexity: O(n + m)
Graph getCond(int cc, const vector<int>& comp, const Graph& g) {
	vector<vector<int>> rev(cc);
	for (int i = 0; i < g.size(); ++i) rev[comp[i]].push_back(i);

	Graph res(cc);
	vector<bool> seen(cc, 0);
	for (int j = 0; j < cc; ++j) {
		for (auto i : rev[j]) {
			for (auto ti : g[i]) {
				int t = comp[ti];
				if (t == j || seen[t]) continue;
				res[j].push_back(t);
				seen[t] = true;
			}
		}
		for (auto t : res[j]) seen[t] = false;
	}
	return res;
}

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
		int getComp(int i) const { return comp[i]; }
		Graph condense() const { return getCond(cc, comp, g); }
};
