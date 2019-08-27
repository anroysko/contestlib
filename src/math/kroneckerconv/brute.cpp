#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

void solve() {
	int m;
	cin >> m;
	int n = 1<<m;

	vector<ll> vec(n);
	for (int i = 0; i < n; ++i) cin >> vec[i];

	vector<vector<ll>> mtx(n, vector<ll>(n, 1));
	for (int b = 1; b < n; b <<= 1) {
		vector<ll> vals(4);
		for (int i = 0; i < 4; ++i) cin >> vals[i];

		for (int y = 0; y < n; ++y) {
			for (int x = 0; x < n; ++x) {
				int x0 = (x & b) ? 1 : 0;
				int y0 = (y & b) ? 2 : 0;
				mtx[y][x] *= vals[y0 + x0];
			}
		}
	}
	/*
	for (int y = 0; y < n; ++y) {
		for (int x = 0; x < n; ++x) {
			cerr << mtx[y][x] << ' ';
		}
		cerr << '\n';
	}
	*/
	vector<ll> res(n, 0);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			res[i] += vec[j] * mtx[j][i];
		}
	}
	for (auto v : res) cout << v << ' '; cout << '\n';
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
