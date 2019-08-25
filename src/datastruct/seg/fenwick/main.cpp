#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, q;
	cin >> n >> q;

	Fenwick fw(n);
	for (int qi = 0; qi < q; ++qi) {
		string op;
		cin >> op;
		if (op == "add") {
			int i;
			ll v;
			cin >> i >> v;
			fw.add(i, v);
		} else if (op == "get") {
			int a, b;
			cin >> a >> b;
			cout << fw.get(a, b) << '\n';
		} else if (op == "search") {
			ll v;
			cin >> v;
			cout << fw.search(v) << '\n';
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
