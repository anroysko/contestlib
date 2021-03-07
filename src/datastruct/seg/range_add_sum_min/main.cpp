#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, q;
	cin >> n >> q;

	SegTree seg(n);
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			seg.rangeAdd(a, b, v);
		} else if (op == "sum") {
			int a, b;
			cin >> a >> b;
			cout << seg.rangeSum(a, b) << '\n';
		} else if (op == "min") {
			int a, b;
			cin >> a >> b;
			cout << seg.rangeMin(a, b) << '\n';
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
