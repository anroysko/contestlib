#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, q;
	cin >> n >> q;

	vector<Line> lines(n);
	for (auto& pr : lines) cin >> pr.a >> pr.b;
	SqrtHull hull(lines);

	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add") {
			int a, b;
			ll d;
			cin >> a >> b >> d;
			hull.rangeAdd(a, b, d);
		} else if (op == "min") {
			int a, b;
			ll x;
			cin >> a >> b >> x;
			ll res = hull.rangeMin(a, b, x);
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
