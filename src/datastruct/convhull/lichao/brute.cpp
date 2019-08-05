#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
};

void solve() {
	int n, q;
	cin >> n >> q;

	vector<ll> xs(n);
	for (int i = 0; i < n; ++i) cin >> xs[i];
	
	vector<Line> lines;
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add_line") {
			ll a, b;
			cin >> a >> b;
			lines.push_back({a, b});
		} else if (op == "min_line") {
			int j;
			cin >> j;
			ll res = lines[0].eval(xs[j]);
			for (int i = 1; i < lines.size(); ++i) res = min(res, lines[i].eval(xs[j]));
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
