class CentroidDecomp {
	private:
		void setBit(int i, int ti, int bit) {
			for (auto& pr : dir[i]) {
				if (pr.first != ti) continue;
				pr.second |= bit;
				return;
			}
			dir[i].emplace_back(ti, bit);
		}
		int calcSiz(int i, int p, int cl) {
			if (lvl[i] < cl) return 0;
			siz[i] = 1;
			for (int ti = 0; ti < g[i].size(); ++ti) {
				if (g[i][ti] != p) siz[i] += calcSiz(g[i][ti], i, cl);
				else if (cl < lvl[i]) setBit(i, ti, 1 << cl);
			}
			return siz[i];
		}
		int centroid(int i, int p, int cl, int n) const {
			if (lvl[i] < cl) return -1;
			for (auto t : g[i]) {
				if (t == p) continue;
				int sub = centroid(t, i, cl, n);
				if (sub != -1) return sub;
			}
			return (2*siz[i] >= n ? i : -1);
		}
	public:
		const vector<vector<int>> g;
		vector<pair<int, int>> par;
		vector<vector<pair<int, int>>> dir;
		vector<int> siz, lvl;

		CentroidDecomp(const vector<vector<int>>& cn) : g(cn), par(cn.size()), dir(cn.size()), siz(cn.size()), lvl(cn.size(), cn.size()) {
			int n = calcSiz(0, -1, n);
			int r = centroid(0, -1, 0, n);
			lvl[r] = 0;
			vector<int> que = {r};
			for (int j = 0; j < n; ++j) {
				int i = que[j];
				int m = calcSiz(i, -1, lvl[i]);
				for (int ei = 0; ei < g[i].size(); ++ei) {
					int t = g[i][ei];
					if (lvl[t] < lvl[i]) continue;

					int tr = centroid(t, i, lvl[i] + 1, siz[t]);
					par[tr] = {i, ei};
					lvl[tr] = lvl[i] + 1;
					que.push_back(tr);
				}
			}
		}
		// Returns list 
		vector<pair<pair<int, int>, int>> getPars(int i) {
			vector<pair<pair<int, int>, int>> res(lvl[i]);
			for (auto pr : dir[i]) {
				for (int v = pr.second; v; v &= v-1) {
					res[__builtin_ctz(v)].second = pr.first;
				}
			}
			for (int j = lvl[i]-1; j >= 0; --j, i = par[i].first) {
				res[j].first = par[i];
			}
			return res;
		}
};
