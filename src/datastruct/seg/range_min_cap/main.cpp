#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, q;
	cin >> n >> q;

	SegTree rm(n);
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "min") {
			int a, b;
			cin >> a >> b;
			cout << rm.rangeMin(a, b) << '\n';
		} else if (op == "cap") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			rm.rangeCap(a, b, v);
		} else if (op == "search") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			cout << rm.search(a, b, v) << '\n';
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
