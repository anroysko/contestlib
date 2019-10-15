#include <iostream>
#include <vector>
#include <bitset>
using namespace std;
using ll = long long;

#include "code.cpp"

void solve() {
	
	int n, q;
	cin >> n >> q;

	const int M = 1000;
	BitGaussian<M> bg;

	for (int i = 0; i < n; ++i) {
		string str;
		cin >> str;
		bitset<M> vec;
		for (int j = 0; j < str.size(); ++j) vec[j] = (str[j] == '1');
		bg.addVec(vec, i);
	}
	for (int qi = 0; qi < q; ++qi) {
		string str;
		cin >> str;
		bitset<M> vec;
		for (int j = 0; j < str.size(); ++j) vec[j] = (str[j] == '1');

		vector<int> opt = bg.minXor(vec);
		for (int j = 0; j < str.size(); ++j) cout << vec[j]; cout << '\n';
		for (auto i : opt) cout << i << ' '; cout << '\n';
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
