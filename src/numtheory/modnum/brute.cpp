#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const int MOD = (int)1e9 + 7;

ll modPow(ll a, ll b) {
	if (b < 0) b += MOD - 1;
	if (b & 1) return a * modPow(a, b-1) % MOD;
	if (b > 0) return modPow(a*a % MOD, b / 2);
	return 1;
}

void solve() {
	ll a, b;
	char op;
	cin >> a >> op >> b;
	
	if (op == '+') cout << (a + b) % MOD << ' ';
	else if (op == '-') cout << (MOD + a - b) % MOD << ' ';
	else if (op == '*') cout << (a * b) % MOD << ' ';
	else if (op == '/') cout << (a * modPow(b, MOD-2)) % MOD << ' ';
	else cout << modPow(a, b) << ' ';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int t;
	cin >> t;
	for (int ti = 0; ti < t; ++ti) {
		solve();
	}
	cout << '\n';
}
