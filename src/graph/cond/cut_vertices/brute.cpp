#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

bool dfs(int i, int ci, vector<int>& comp, const vector<bool>& ban, const vector<vector<int>>& g) {
	if (ban[i] || comp[i] != -1) return false;
	comp[i] = ci;
	for (auto t : g[i]) dfs(t, ci, comp, ban, g);
	return true;
}

void solve() {
	int n, m;
	cin >> n >> m;
	vector<vector<int>> g(n);
	for (int j = 0; j < m; ++j) {
		int a, b;
		cin >> a >> b;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	vector<bool> cutv(n, 0), ban(n, 0);
	for (int x = 0; x < n; ++x) {
		ban[x] = true;

		int cou = 0;
		vector<int> comp(n, -1);
		for (auto t : g[x]) cou += dfs(t, 1, comp, ban, g);
		if (cou >= 2) cutv[x] = true;

		ban[x] = false;
	}

	int cc = 0;
	vector<int> comp(n, -1);
	for (int i = 0; i < n; ++i) {
		if (cutv[i]) {
			comp[i] = cc;
			++cc;
		} else {
			cc += dfs(i, cc, comp, cutv, g);
		}
	}

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) cout << (comp[i] == comp[j]);
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
