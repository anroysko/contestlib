#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

int main() {
	int n, q;
	cin >> n >> q;

	vector<ll> vals(n);
	for (int i = 0; i < n; ++i) cin >> vals[i];
	
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "range_sum") {
			int a, b;
			cin >> a >> b;
			ll res = 0;
			for (int i = a; i <= b; ++i) res += vals[i];
			cout << res << '\n';
		} else if (op == "range_add") {
			int a, b;
			ll v;
			cin >> a >> b >> v;
			for (int i = a; i <= b; ++i) vals[i] += v;
		}
	}
}
