#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, q;
	cin >> n >> q;

	vector<ll> vals(n), weights(n);
	for (int i = 0; i < n; ++i) cin >> vals[i];
	for (int i = 0; i < n; ++i) cin >> weights[i];

	SegTree seg(vals, weights);
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			seg.add(a, b, v);
		} else if (op == "get") {
			int a, b;
			cin >> a >> b;
			auto res = seg.get(a, b);
			cout << res.first << ' ' << res.second << '\n';
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
