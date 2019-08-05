#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
};

void solve() {
	int q;
	cin >> q;

	vector<Line> lines;
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add_line") {
			ll a, b;
			cin >> a >> b;
			lines.push_back({a, b});
		} else if (op == "min_line") {
			ll x;
			cin >> x;
			ll res = lines[0].eval(x);
			for (int i = 1; i < lines.size(); ++i) res = min(res, lines[i].eval(x));
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
