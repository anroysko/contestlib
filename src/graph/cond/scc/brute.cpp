#include <iostream>
#include <vector>
#include <tuple>
using namespace std;
using ll = long long;

void dfs(int i, vector<bool>& visit, vector<vector<int>>& g) {
	if (visit[i]) return;
	visit[i] = true;
	for (auto t : g[i]) dfs(t, visit, g);
}

void solve() {
	int n, m;
	cin >> n >> m;
	vector<vector<int>> g(n);
	for (int j = 0; j < m; ++j) {
		int a, b;
		cin >> a >> b;
		g[a].push_back(b);
	}
	vector<vector<bool>> reach(n, vector<bool>(n, 0));
	for (int i = 0; i < n; ++i) dfs(i, reach[i], g);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) cout << ((bool)reach[i][j] && (bool)reach[j][i]);
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
