#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "debrujin.cpp"

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, k;
	cin >> n >> k;
	
	vector<int> res = debrujin(n, k);
	for (int& v : res) cout << v << ' '; cout << '\n';
}
