#include <iostream>
#include <vector>
using namespace std;

#include "code.cpp"

void solve() {
	string str;
	cin >> str;

	vector<int> z = zAlgo(str);
	for (auto v : z) cout << v << ' '; cout << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) solve();
}
