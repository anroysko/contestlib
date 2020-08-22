#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, q;
	cin >> n >> q;

	vector<ll> xs(n);
	for (int i = 0; i < n; ++i) cin >> xs[i];

	LiChao lc(xs);
	for (int qi = 0; qi < q; ++qi) {
		string q;
		cin >> q;
		if (q == "add_line") {
			ll a, b; // ax + b
			cin >> a >> b;
			lc.addLine({a, b});
		} else if (q == "min_line") {
			int j;
			cin >> j;
			cout << lc.minVal(j) << '\n';
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
