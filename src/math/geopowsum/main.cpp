#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll MOD = 998244353;

#include "code.cpp"

void solve() {
	ll n, r;
	int k;
	cin >> n >> r >> k;
	
	ll res = geoPowSum(n, r, k);
	cout << res << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) solve();
}

