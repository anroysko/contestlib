#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

#include "../mult/code.cpp"
#include "../div/code.cpp"
#include "code.cpp"

void solve() {
	int t;
	cin >> t;
	if (t == 0) {
		int n, m;
		cin >> n >> m;

		vector<int> pol(n), xs(m);
		for (int& v : pol) cin >> v;
		for (int& v : xs) cin >> v;

		auto vals = multiPointEval<NTTP[0]>(pol, xs);
		for (auto v : vals) cout << v << ' '; cout << '\n';
	} else {
		int n, a, b, k;
		cin >> n >> a >> b >> k;

		vector<int> pol(n);
		for (int& v : pol) cin >> v;

		auto vals = chirpzEval<NTTP[0]>(pol, a, b, k);
		for (auto v : vals) cout << v << ' '; cout << '\n';
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
