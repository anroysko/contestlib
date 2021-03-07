#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll INF = 4 * (ll)1e18;

struct Line {
	ll a, b;
	ll eval(ll x) const { return a*x + b; }
};

void solve(bool db) {
	int n, q;
	cin >> n >> q;
	if (db) cout << n << ' ' << q << '\n';

	vector<Line> lines(n);
	for (auto& pr : lines) {
		cin >> pr.a >> pr.b;
		if (db) cout << pr.a << ' ' << pr.b << '\n';
	}

	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add") {
			int a, b;
			ll d;
			cin >> a >> b >> d;
			for (int i = a; i <= b; ++i) lines[i].b += lines[i].a * d;
			if (db) cout << op << ' ' << a << ' ' << b << ' ' << d << '\n';
		} else if (op == "min") {
			int a, b;
			ll x;
			cin >> a >> b >> x;
			if (db) cout << op << ' ' << a << ' ' << b << ' ' << x << '\n';
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) {
		solve(ti == 65);
	}
}
