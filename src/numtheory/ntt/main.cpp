#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, m, p;
	cin >> n >> m >> p;

	vector<int> a(n), b(m);
	for (int& v : a) cin >> v;
	for (int& v : b) cin >> v;

	vector<int> c = polyMultMod(a, b, p);
	for (auto v : c) cout << v << ' '; cout << '\n';
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
