#include <iostream>
#include <vector>
#include <tuple>
using namespace std;
using ll = long long;

void dfs(int i, int ci, vector<int>& comp, vector<vector<int>>& g) {
	comp[i] = ci;
	for (auto t : g[i]) {
		if (comp[t] == -1) dfs(t, ci, comp, g);
	}
}

void solve() {
	int n, m;
	cin >> n >> m;
	vector<pair<int, int>> edges(m);
	for (int j = 0; j < m; ++j) {
		int a, b;
		cin >> a >> b;
		edges[j] = {a, b};
	}
	vector<vector<bool>> res(n, vector<bool>(n, 1));
	for (int x = -1; x < m; ++x) {
		vector<vector<int>> g(n);
		for (int j = 0; j < m; ++j) {
			if (j == x) continue;
			int a, b;
			tie(a, b) = edges[j];
			g[a].push_back(b);
			g[b].push_back(a);
		}

		vector<int> comp(n, -1);
		for (int i = 0; i < n; ++i) {
			if (comp[i] == -1) dfs(i, i, comp, g);
		}

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				if (comp[i] != comp[j]) res[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) cout << res[i][j];
		cout << '\n';
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) {
		solve();
	}
}
