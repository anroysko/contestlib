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

	vector<int> vals = multiPointEval<NTTP[0]>(pol, xs);
	for (auto v : vals) cout << v << ' '; cout << '\n';
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
