#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

#include "code.cpp"

void solve() {
	string str;
	cin >> str;
	str.push_back('$');

	vector<int> sa, rev;
	tie(sa, rev) = suffixArray(str);
	for (auto v : sa) cout << v << ' '; cout << '\n';
	for (auto v : rev) cout << v << ' '; cout << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) solve();
}
