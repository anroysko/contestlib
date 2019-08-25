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
			int i;
			ll v;
			cin >> i >> v;
			vals[i] += v;
		} else if (op == "get") {
			int a, b;
			cin >> a >> b;

			ll res = 0;
			for (int j = a; j <= b; ++j) res += vals[j];
			cout << res << '\n';
		} else if (op == "search") {
			ll v;
			cin >> v;

			int res = -1;
			ll sum = 0;
			for (int j = 0; j < n; ++j) {
				sum += vals[j];
				if (sum <= v) res = j;
			}
			cout << res << '\n';
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
