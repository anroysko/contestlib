#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "gen_match.cpp"

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m;
	cin >> n >> m;

	GeneralMatching match(n);
	for (int i = 0; i < m; ++i) {
		int a, b, u;
		cin >> a >> b >> u;
		match.addEdge(a, b, u);
	}
	vector<int> res = match.getMatching();
	cout << "Matching (" << res.size() << " edges): ";
	for (auto i : res) cout << i << ' '; cout << '\n';
}
