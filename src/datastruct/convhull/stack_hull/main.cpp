#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int q;
	cin >> q;

	ConvHull hull;
	for (int qi = 0; qi < q; ++qi) {
		string q;
		cin >> q;
		if (q == "add_line") {
			ll a, b; // ax + b
			cin >> a >> b;
			hull.addLine({a, b});
		} else if (q == "min_line") {
			ll x;
			cin >> x;
			cout << hull.minLine(x).eval(x) << '\n';
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
