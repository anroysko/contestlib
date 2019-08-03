class HLD {
	private:
		vector<int> par, siz, cmp, ind;

		void dfs1(int i, vector<vector<int>>& g) {
			for (auto& t : g[i]) {
				if (t == par[i]) continue;
				par[t] = i;
				dfs1(t, g);
				siz[i] -= siz[t];
				if (siz[t] > siz[g[i][0]]) swap(t, g[i][0]);
			}
			siz[i] *= -1;
		}
		void dfs2(int i, int& x, const vector<vector<int>>& g) {
			ind[i] = x;
			++x;

			for (auto t : g[i]) {
				if (t == par[i]) continue;
				cmp[t] = (x == ind[i]+1 ? cmp[i] : t);
				dfs2(t, x, g);
			}
		}
	public:
		// Assumes the tree is connected and r is the root
		HLD(vector<vector<int>> g, int r = 0)
				: par(g.size(), -1), siz(g.size(), -1), cmp(g.size(), r), ind(g.size()) {
			dfs1(r, g);
			int x = 0;
			dfs2(r, x, g);
		}
		// Returns intervals corresponding to the path between a and b, not necessarily in order
		vector<pair<int, int>> path(int a, int b) const {
			vector<pair<int, int>> res;
			for (;; b = par[cmp[b]]) {
				if (ind[b] < ind[a]) swap(a, b);
				if (ind[cmp[b]] <= ind[a]) {
					res.push_back({ind[a], ind[b]});
					return res;
				}
				res.push_back({ind[cmp[b]], ind[b]});
			}
		}
		// Returns interval corresponding to the subtree of node i
		pair<int, int> subtree(int i) const {
			return {ind[i], ind[i] + siz[i] - 1};
		}
		// Returns LCA of nodes a and b
		int lca(int a, int b) const {
			for (;; b = par[cmp[b]]) {
				if (ind[b] < ind[a]) swap(a, b);
				if (ind[cmp[b]] <= ind[a]) return a;
			}
		}
};
