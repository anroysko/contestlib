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

// Class for finding cut vertices in an undirected graph. A cut vertex is a vertex
// such that if it is removed, the remaining graph is disconnected.
// Time complexity (of construction): O(n + m)
class CutVertices {
	private:
		const Graph& g;
		vector<int> comp, dep, cutv;
		int di = 0, cc = 0;

		void flood(int i) {
			comp[i] = cc;
			for (auto t : g[i]) {
				if (!cutv[i] && !cutv[t] && comp[t] == -1) flood(t);
			}
		}
		int dfs(int i, int p) {
			dep[i] = dep[p] + 1;
			int res = dep[i];

			for (auto t : g[i]) {
				if (dep[t]) res = min(res, dep[t]);
				else {
					int sub = dfs(t, i);
					if (sub >= dep[i]) ++cutv[i];
					else res = min(res, sub);
				}
			}
			cutv[i] = (cutv[i] > (i == p));
			return res;
		}
	public:
		CutVertices(const Graph& gr) : g(gr), comp(gr.size(), -1), dep(gr.size(), 0), cutv(gr.size(), 0) {
			for (int i = 0; i < g.size(); ++i) {
				if (! dep[i]) dfs(i, i);
				if (comp[i] == -1) {
					flood(i);
					++cc;
				}
			}
		}
		bool isCutv(int i) const { return cutv[i]; }
		int getComp(int i) const { return comp[i]; }
		Graph condense() const { return getCond(cc, comp, g); }
};
