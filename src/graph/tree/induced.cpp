class HLD {
	private:
		vector<int> par, siz, cmp, ind, dep;

		int dfs1(int i, vector<vector<int>>& g) {
			for (auto& t : g[i]) {
				if (t == par[i]) continue;
				par[t] = i;
				dep[t] = dep[i] + 1;
				siz[i] += dfs1(t, g);
				if (siz[t] > siz[g[i][0]] || g[i][0] == par[i]) swap(t, g[i][0]);
			}
			return siz[i];
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
				: par(g.size(), -1), siz(g.size(), 1), cmp(g.size(), r), ind(g.size()), dep(g.size(), 0) {
			dfs1(r, g);
			int x = 0;
			dfs2(r, x, g);
		}
		// Returns intervals corresponding to the path between a and b in descending order
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
		int lca(int a, int b) const {
			for (;; b = par[cmp[b]]) {
				if (ind[b] < ind[a]) swap(a, b);
				if (ind[cmp[b]] <= ind[a]) return a;
			}
		}
		int dist(int a, int b) const {
			return dep[a] + dep[b] - 2 * dep[lca(a, b)];
		}

		// Builds the tree induced by the given vertices
		// ith vertex of induced tree corresponds to ith vertex in the input
		// (Vertices in the induced tree not in the input will be added to it)
		vector<vector<pair<int, int>>> inducedTree(vector<int>& v) const {
			int m = v.size();
			vector<pair<int, int>> ord(m);
			for (int j = 0; j < m; ++j) ord[j] = {ind[v[j]], j};
			sort(ord.begin(), ord.end());

			vector<int> sta;
			vector<vector<pair<int, int>>> g(m);
			for (auto pr : ord) {
				int t = pr.second;
				if (! sta.empty()) {
					int x = lca(v[sta.back()], v[t]);
					while(dep[x] < dep[v[sta.back()]]) {
						int i = sta.back();
						sta.pop_back();
						if (sta.empty() || dep[v[sta.back()]] < x) {
							sta.push_back(v.size());
							v.push_back(x);
						}
						int j = sta.back();
						int d = dep[v[i]] - dep[v[j]];
						g[i].emplace_back(j, d);
						g[j].emplace_back(i, d);
					}
				}
				sta.push_back(t);
			}
			// Clean up: pop rest of stack, and possibly remove LCA
			while(sta.size() > 1) {
				int i = sta.back();
				sta.pop_back();
				int j = sta.back();
				int d = dep[v[i]] - dep[v[j]];
				g[i].emplace_back(j, d);
				g[j].emplace_back(i, d);
			}
			if (sta[0] >= m && g[sta[0]].size() == 2) {
				int k = sta[0];
				int i = g[k][0].first;
				int j = g[k][1].first;
				int d = g[k][0].second + g[k][1].second;
				g[i].emplace_back(j, d);
				g[j].emplace_back(i, d);
				v.erase(v.begin() + k);
				g.erase(g.begin() + k);
			}
			return g;
		}
};
