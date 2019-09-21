#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

void solve() {
	int n, q;
	cin >> n >> q;

	vector<ll> vals(n, 4*(ll)1e18);
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "min") {
			int a, b;
			cin >> a >> b;
			ll res = vals[a];
			for (int i = a+1; i <= b; ++i) res = min(res, vals[i]);
			cout << res << '\n';
		} else if (op == "cap") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			for (int i = a; i <= b; ++i) vals[i] = min(vals[i], v);
		} else if (op == "search") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			for (; a <= b; ++a) {
				if (vals[a] <= v) break;
			}
			cout << a << '\n';
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
