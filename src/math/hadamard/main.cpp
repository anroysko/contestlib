#include <iostream>
#include <vector>
#include <valarray>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	int n, h = 0;
	cin >> n;
	while((1<<h) < n) ++h;

	vector<ll> a(1<<h, 0), b(1<<h, 0);
	for (int i = 0; i < n; ++i) cin >> a[i];
	for (int i = 0; i < n; ++i) cin >> b[i];

	vector<ll> c1 = xorConv(a, b, h);
	vector<ll> c2 = andConv(a, b, h);
	vector<ll> c3 = orConv(a, b, h);
	for (int i = 0; i < n; ++i) cout << c1[i] << ' '; cout << '\n';
	for (int i = 0; i < n; ++i) cout << c2[i] << ' '; cout << '\n';
	for (int i = 0; i < n; ++i) cout << c3[i] << ' '; cout << '\n';
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
