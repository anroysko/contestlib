#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	string t;
	cin >> t;

	int n;
	cin >> n;
	vector<ll> f(1<<n);
	for (int i = 0; i < (1<<n); ++i) cin >> f[i];

	if (t == "mobius") {
		ll r;
		cin >> r;
		auto res = mobius(n, f, r);
		for (auto v : res) cout << v << ' '; cout << '\n';
	} else if (t == "tieredMobius") {
		auto res = tieredMobius(n, f);
		for (int k = 0; k <= n; ++k) {
			for (auto v : res[k]) cout << v << ' '; cout << '\n';
		}
	} else if (t == "subsetConv") {
		vector<ll> g(1<<n);
		for (int i = 0; i < (1<<n); ++i) cin >> g[i];
		auto res = subsetConv(n, f, g);
		for (auto v : res) cout << v << ' '; cout << '\n';
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
