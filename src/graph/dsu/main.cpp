#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, m;
	cin >> n >> m;
	vector<vector<int>> g(n);
	for (int i = 0; i < m; ++i) {
		int a, b;
		cin >> a >> b;
		g[a].push_back(b);
		g[b].push_back(a);
	}

	Bridges bd(g);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) cout << (bd.getComp(i) == bd.getComp(j));
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
