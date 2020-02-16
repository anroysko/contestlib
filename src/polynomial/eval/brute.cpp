#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

#include "../mult/code.cpp"
#include "../div/code.cpp"
#include "code.cpp"

void eval(vector<int>& pol, vector<int>& xs) {
	for (int i = 0; i < xs.size(); ++i) {
		ll res = 0;
		for (int j = (int)pol.size() - 1; j >= 0; --j) {
			res = (res * xs[i] + pol[j]) % NTTP[0];
		}
		cout << res << ' ';
	}
	cout << '\n';
}

void solve() {
	int t;
	cin >> t;
	if (t == 0) {
		int n, m;
		cin >> n >> m;

		vector<int> pol(n), xs(m);
		for (int& v : pol) cin >> v;
		for (int& v : xs) cin >> v;
		eval(pol, xs);
	} else {
		int n, z, k;
		cin >> n >> z >> k;
		
		vector<int> pol(n), xs(k);
		for (int& v : pol) cin >> v;
		for (int i = 0; i < k; ++i) xs[i] = modPow(z, i, NTTP[0]);
		eval(pol, xs);
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
