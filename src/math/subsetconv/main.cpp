#include <iostream>
#include <vector>
#include <array>
#include <bitset>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	string t;
	cin >> t;

	if (t == "binaryConv") {
		const int N = 10;
		const int M = 1 << N;
		BinaryConv<N> conv;
		
		string x, y;
		cin >> x >> y;

		bitset<M> f, g;
		for (int i = 0; i < M; ++i) f[i] = (x[i] == '1');
		for (int i = 0; i < M; ++i) g[i] = (y[i] == '1');
		bitset<M> fg = conv.apply(f, g);
		cout << fg << '\n';
	} else {
		int n;
		cin >> n;

		vector<ll> f(1<<n);
		for (int i = 0; i < (1<<n); ++i) cin >> f[i];

		if (t == "mobius") {
			int r;
			cin >> r;
			mobius(f, r == -1);
			for (auto v : f) cout << v << ' '; cout << '\n';
		} else if (t == "tieredMobius") {
			auto res = tieredMobius(f, n);
			for (int k = 0; k <= n; ++k) {
				for (auto v : res[k]) cout << v << ' '; cout << '\n';
			}
		} else if (t == "subsetConv") {
			vector<ll> g(1<<n);
			for (int i = 0; i < (1<<n); ++i) cin >> g[i];
			auto res = subsetConv(f, g, n);
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
