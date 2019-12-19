#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const ll MOD = 998244353;

ll modPow(ll a, ll b) {
	if (b & 1) return a * modPow(a, b^1) % MOD;
	if (b == 0) return 1;
	return modPow(a*a % MOD, b >> 1);
}

void solve() {
	ll n, r;
	int k;
	cin >> n >> r >> k;
	
	ll res = 0;
	for (int i = 0; i < n; ++i) {
		res = (res + modPow(r, i) * modPow(i, k)) % MOD;
	}
	cout << res << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) solve();
}

