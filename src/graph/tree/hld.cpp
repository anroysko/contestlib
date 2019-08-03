class HLD {
	private:
		vector<int> par;
		vector<int> siz;
		vector<int> cmp;
		vector<int> ind;
	public:
		HLD(const vector<int>& p) : par(p), siz(p.size(), 1), cmp(p.size(), 0), ind(p.size()) {
			int n = par.size();
			for (int i = n-1; i > 0; --i) siz[par[i]] += siz[i];

			vector<int> aux = siz;
			for (int i = 1; i < n; ++i) {
				if (2*siz[i] >= siz[par[i]]) {
					ind[i] = ind[par[i]] + 1;
					cmp[i] = cmp[par[i]];
				} else {
					aux[par[i]] -= siz[i];
					ind[i] = aux[par[i]];
					cmp[i] = i;
				}
				aux[i] += ind[i];
			}
		}
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
		pair<int, int> subtree(int i) const {
			return {ind[i], ind[i] + siz[i] - 1};
		}
		int lca(int a, int b) const {
			for (;; b = par[cmp[b]]) {
				if (ind[b] < ind[a]) swap(a, b);
				if (ind[cmp[b]] <= ind[a]) return a;
			}
		}
};
