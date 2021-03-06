#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll MOD = 998244353;

#include "code.cpp"

void solve() {
	int n, m;
	cin >> n >> m;

	vector<int> a(n), b(m);
	for (int& v : a) cin >> v;
	for (int& v : b) cin >> v;

	vector<int> c(n+m-1, 0);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			c[i+j] = (c[i+j] + (ll)a[i] * b[j]) % MOD;
		}
	}
	for (auto v : c) cout << v << ' '; cout << '\n';
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
