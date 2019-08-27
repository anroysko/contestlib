#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int m;
	cin >> m;
	int n = 1<<m;

	vector<ll> vec(n);
	for (int i = 0; i < n; ++i) cin >> vec[i];

	vector<vector<ll>> mats(m, vector<ll>(4));
	for (int j = 0; j < m; ++j) {
		for (int i = 0; i < 4; ++i) cin >> mats[j][i];
	}

	vector<ll> res = kronecker(vec, mats);
	for (auto v : res) cout << v << ' '; cout << '\n';
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
