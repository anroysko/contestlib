#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

#include "../mult/code.cpp"

void solve() {
	int n, m;
	cin >> n >> m;

	vector<int> a(n), b(m);
	for (int& v : a) cin >> v;
	for (int& v : b) cin >> v;

	vector<int> c(n-m+1);
	for (int i = n-m; i >= 0; --i) {
		c[i] = a[i + (m-1)];
		for (int j = 0; j < m; ++j) {
			a[i + j] = mSub(a[i + j], (ll)b[j] * c[i] % NTTP[0], NTTP[0]);
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
