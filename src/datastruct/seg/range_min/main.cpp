#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, q;
	cin >> n >> q;
	vector<ll> ini(n);
	for (int i = 0; i < n; ++i) cin >> ini[i];

	RangeMin<ll> rm(ini);
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "get") {
			int a, b;
			cin >> a >> b;
			cout << rm.get(a, b) << '\n';
		} else if (op == "set") {
			int i;
			ll v;
			cin >> i >> v;
			rm.set(i, v);
		} else if (op == "find") {
			int i;
			ll v;
			cin >> i >> v;
			cout << rm.find(i, v) << '\n';

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
