#include <iostream>
#include <vector>
using namespace std;

void solve() {
	string str, pat;
	cin >> str >> pat;

	int n = str.size();
	vector<int> matches(n, 0);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < pat.size() && i+j < n; ++j) {
			if (str[i+j] != pat[j]) break;
			else matches[i+j] = max(matches[i+j], j+1);
		}
	}
	for (auto v : matches) cout << v << ' '; cout << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) solve();
}
