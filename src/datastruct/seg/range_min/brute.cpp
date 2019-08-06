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
		if (op == "get") {
			int a, b;
			cin >> a >> b;
			ll res = vals[a];
			for (int i = a+1; i <= b; ++i) res = min(res, vals[i]);
			cout << res << '\n';
		} else if (op == "set") {
			int i;
			ll v;
			cin >> i >> v;
			vals[i] = v;
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
