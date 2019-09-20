#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

void solve() {
	int n, q;
	cin >> n >> q;

	vector<ll> vals(n), weights(n);
	for (int i = 0; i < n; ++i) cin >> vals[i];
	for (int i = 0; i < n; ++i) cin >> weights[i];

	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			for (int i = a; i <= b; ++i) vals[i] += v;
		} else if (op == "get") {
			int a, b;
			cin >> a >> b;
			ll res = 2*(ll)1e18;
			ll cou = 0;
			for (int i = a; i <= b; ++i) {
				if (res > vals[i]) {
					res = vals[i];
					cou = 0;
				}
				if (res == vals[i]) cou += weights[i];
			}
			cout << res << ' ' << cou << '\n';
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
