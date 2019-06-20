#include <iostream>
#include <utility>
using namespace std;
using ll = long long;
const int MOD = 1e9 + 7;

// Time complexity: log(min(a, b))
// Returns: {x, y} s.t. ax + by = gcd(a, b) (mod p)
pair<ll, ll> extEucMod(ll a, ll b, ll p) {
	if (b == 0) return {1, 0};
	ll m = a / b;
	auto sub = extEucMod(b, a - b * m, p);
	return {sub.second, (sub.first - m*sub.second) % p};
}
ll modInv(ll a, ll p) {
	ll res = extEucMod(p, a, p).second;
	return (res < 0 ? res + p : res);
}

int main() {
	ll a, p;
	cin >> a >> p;
	cout << a << ' ' << modInv(a, p) << ' ' << a*modInv(a, p) % p << '\n';
}
