#include <iostream>
using namespace std;
typedef long long ll;

// Returns x and y such that ax + by = 1
// Time complexity: O(log min(a, b))
pair<ll, ll> extEuc(ll a, ll b) {
	if (b == 0) return {1, 0};
	ll m = a / b;
	auto sub = extEuc(b, a - m*b);
	return {sub.second, sub.first - m*sub.second};
}

// Returns y such that vy = gcd(v, mod) (mod)
// Time complexity: O(log v)
ll modInv(ll v, ll mod) {
	return extEuc(mod, v).second % mod;
}

// There exists unique 0 <= x < \prod ps[i], such that
// x = vs[i] (ps[i]) for 0 <= i < n. Returns x % mod.
// time complexity: O(n^2 + n log mod)
// If \prod ps[i] \leq mod: O(n log mod)
ll crt(const vector<ll>& vs, const vector<ll>& ps, ll mod) {
	int n = vs.size();
	ll res = 0;
	ll prod = 1;
	for (int i = 0; i < n; ++i) prod = (prod * ps[i]) % mod;
	for (int i = 0; i < n; ++i) {
		ll mult = (prod * modInv(ps[i], mod)) % mod;
		ll inv = 1; // = modInv(mult, ps[i]); // If \prod ps[i] \leq mod
		for (int j = 0; j < n; ++j) {
			if (i == j) continue;
			inv = (inv * ps[j]) % ps[i];
		}
		inv = modInv(inv, ps[i]);
		mult = (inv * mult) % mod;
		res = (res + vs[i] * mult) % mod;
	}
	return res;
}

// Generalized chinese remainder theorem (CRT).
// Works the same as crt, except now ps's don't
// have to be coprime, And lca(ps) must not overflow
// Time complexity: 
ll genCRT() {

}

int main() {
	
}
