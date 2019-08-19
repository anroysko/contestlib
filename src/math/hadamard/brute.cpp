#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

void solve() {
	int n;
	cin >> n;
	vector<ll> a(n), b(n), c1(n, 0), c2(n, 0), c3(n, 0);
	for (int i = 0; i < n; ++i) cin >> a[i];
	for (int i = 0; i < n; ++i) cin >> b[i];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			ll p = a[i]*b[j];
			if ((i^j) < n) c1[i^j] += p;
			if ((i&j) < n) c2[i&j] += p;
			if ((i|j) < n) c3[i|j] += p;
		}
	}
	for (auto v : c1) cout << v << ' '; cout << '\n';
	for (auto v : c2) cout << v << ' '; cout << '\n';
	for (auto v : c3) cout << v << ' '; cout << '\n';
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
