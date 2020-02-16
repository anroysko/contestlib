#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

#include "../mult/code.cpp"
#include "../div/code.cpp"
#include "code.cpp"

void solve() {
	int n, m;
	cin >> n >> m;

	vector<int> pol(n), xs(m);
	for (int& v : pol) cin >> v;
	for (int& v : xs) cin >> v;

	for (int i = 0; i < m; ++i) {
		ll res = 0;
		for (int j = n-1; j >= 0; --j) {
			res = (res * xs[i] + pol[j]) % NTTP[0];
		}
		cout << res << ' ';
	}
	cout << '\n';
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
