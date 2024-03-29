class Hopcroft {
	private:
		const int n, m;
		vector<int> pr;
		vector<vector<int>> g;

		bool dfs(int i, vector<int>& dist) {
			for (int t : g[i]) {
				if (pr[t] == -1 || (dist[pr[t]] == dist[i] + 1 && dfs(pr[t], dist))) {
					pr[t] = i;
					return 1;
				}
			}
			dist[i] = -1;
			return 0;
		}
	public:
		Hopcroft(int n_, int m_) : n(n_), m(m_), pr(m_, -1), g(n_) {}
		void addEdge(int i, int j) { g[i].push_back(j); }

		vector<pair<int, int>> match() {
			vector<int> que, dist(n);
			que.reserve(n);
			while(true) {
				for (int i = 0; i < n; ++i) dist[i] = 0;
				for (int i = 0; i < m; ++i) { if (pr[i] != -1) dist[pr[i]] = -1; }
				for (int i = 0; i < n; ++i) { if (dist[i] == 0) que.push_back(i); }

				int found = -1;
				for (int j = 0; j < que.size() && found == -1; ++j) {
					for (int y : g[que[j]]) {
						if (pr[y] == -1) found = dist[que[j]];
						else if (dist[pr[y]] == -1) {
							dist[pr[y]] = dist[que[j]] + 1;
							que.push_back(pr[y]);
						}
					}
				}
				if (found == -1) break;

				for (int j = (int)que.size() - 1; j >= 0 && dist[que[j]] > found; --j) dist[que[j]] = -1;
				for (int j = 0; j < que.size() && dist[que[j]] == 0; ++j) dfs(que[j], dist);
				que.clear();
			}
			vector<pair<int, int>> res;
			for (int i = 0; i < m; ++i) { if (pr[i] != -1) res.emplace_back(pr[i], i); }
			return res;
		}
};
