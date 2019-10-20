#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

void solve() {
	int n, q;
	cin >> n >> q;

	vector<ll> vals(n, 0);
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			for (int i = a; i <= b; ++i) vals[i] += v;
		} else if (op == "min") {
			int a, b;
			cin >> a >> b;
			ll res = 4 * (ll)1e18;
			for (int i = a; i <= b; ++i) res = min(res, vals[i]);
			cout << res << '\n';
		} else if (op == "find") {
			int a, b;
			ll v;
			cin >> a >> b >> v;

			int i = a;
			for (i = a; i <= b; ++i) {
				if (vals[i] <= v) break;
			}
			cout << i << '\n';
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
