#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, q;
	cin >> n >> q;

	RangeMin<ll> rm(vector<ll>(n, 0));
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
