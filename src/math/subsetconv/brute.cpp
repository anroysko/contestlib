#include <iostream>
#include <vector>
#include <array>
#include <bitset>
using namespace std;
using ll = long long;

vector<ll> subsetConv(int n, const vector<ll>& f, const vector<ll>& g) {
	vector<ll> res(1<<n, 0);
	for (int m = 0; m < (1<<n); ++m) {
		for (int s = m;; s=(s-1)&m) {
			res[m] += f[s] * g[m^s];
			if (! s) break;
		}
	}
	return res; // res[m] = \sum_{s \subset m} f[s] g[m^s]
}
vector<ll> mobius(int n, const vector<ll>& f, ll r = 1) {
	vector<ll> g(1<<n, 1);
	for (int m = 1; m < (1<<n); ++m) g[m] = r * g[m&(m-1)];
	return subsetConv(n, f, g); // res[m] = \sum_{s \subset m} f[s] r^|m^s|
}
vector<vector<ll>> tieredMobius(int n, const vector<ll>& f) {
	vector<vector<ll>> res(n+1, vector<ll>(1<<n, 0));
	for (int m = 0; m < (1<<n); ++m) {
		for (int s = m;; s=(s-1)&m) {
			res[__builtin_popcount(s)][m] += f[s];
			if (! s) break;
		}
	}
	return res; // res[k][m]: \sum_{s \subset m, |s| = k} f[s]
}

const int N = 10;
const int M = 1 << N;

void solve() {
	string t;
	cin >> t;

	if (t == "binaryConv") {
		string x, y;
		cin >> x >> y;

		bitset<M> f, g;
		for (int i = 0; i < M; ++i) f[i] = (x[i] == '1');
		for (int i = 0; i < M; ++i) g[i] = (y[i] == '1');

		bitset<M> fg;
		for (int m = 0; m < M; ++m) {
			fg[m] = f[0] & g[m];
			for (int j = m; j; j = (j-1) & m) {
				if (f[j] & g[m^j]) fg[m] = 1 - fg[m];
			}
		}
		cout << fg << '\n';
	} else {
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
