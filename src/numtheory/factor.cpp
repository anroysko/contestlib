#include <iostream>
#include <vector>
#include <random>
#include <utility>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef __int128 lll;

lll modPow(lll a, lll b, lll c) {
	if (b & 1) return (a * modPow(a, b-1, c)) % c;
	if (b == 0) return 1;
	lll sub = modPow(a, b>>1, c);
	return (sub * sub) % c;
}

bool isWitness(lll w, lll even, lll odd, lll val) {
	w = modPow(w, odd, val);
	if (w == 1) return 0;
	for (int i = 0; i < even; ++i) {
		lll nw = (w * w) % val;
		if (nw == 1) return (w != val-1);
		w = nw;
	}
	return 1;
}

// Deterministic Miller-Rabin
// https://miller-rabin.appspot.com/
// Time Complexity: O(log(val))
// Preconditions: 128-bit integers are required for testing 64-bit numbers.
bool isPrime(ll val) {
	if (val <= 1 || ((val & 1) == 0 && val > 2)) return 0;
	ll even = 0;
	ll odd = val-1;
	while(!(odd & 1)) {
		++even;
		odd >>= 1;
	}
	ll b[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
	for (int i = 0; i < 7; ++i) {
		ll w = b[i] % val;
		if (w == 0) return 1;
		if (isWitness(w, even, odd, val)) return 0;
	}
	return 1;
}

// find 1 < x < v such that x | v.
// Has the optimizations of moving only one pointer,
// and not taking gcd after every move
// Time Complexity: expected O(sqrt(x)), worst case O(v^(1/4)) for any factor
// Preconditions: v is not prime
ll pollardRho(ll v) {
	for (int c = 1;; ++c) {
		ll x = (rand() + rand() * (ll)1e9) % v;
		ll y = x;
		ll ph = 1;
		int len = 7;
		for (int i = 0;; ++i) {
			y = ((lll)y * y + c) % v;
			ph = ((lll)ph * (abs(x-y))) % v;
			if ((i & 15) == 0) {
				ll g = __gcd(ph, v);
				if (g != 1) {
					if (g == v) break;
					else return g;
				}
				ph = 1;
				if ((i & len) == 0) {
					x = y;
					len = 2*len+1;
				}
			}
		}
	}
}

// Factor v into pairs pi, xi, such that v = \prod pi^xi
// Time Complexity: O(v^(1/4) * \sum_i xi)
// Returns: vector of pairs {pi, xi}
// This should only be used for large integers. Use sieve for factoring ones up to ~1e7
vector<pair<ll, int>> factor(ll v) {
	if (v <= 1) return {};
	vector<ll> ps; // Primes that divide v
	vector<ll> fl = {v}; // List to factor
	while(! fl.empty()) {
		v = fl.back();
		fl.pop_back();
		if (isPrime(v)) ps.push_back(v);
		else {
			ll d = pollardRho(v);
			fl.push_back(v / d);
			fl.push_back(d);
		}
	}
	sort(ps.begin(), ps.end());

	vector<pair<ll, int>> res;
	for (int i = 0; i < ps.size(); ++i) {
		if ((i > 0) && (ps[i-1] == ps[i])) {
			++res.back().second;
		} else {
			res.push_back({ps[i], 1});
		}
	}
	return res;
}

int main() {
	ll val1 = (ll)1e8 + 7;
	ll val2 = (ll)1e9 + 7;
	ll val3 = 2*2*3*7*13*41;
	auto res1 = factor(val1 * val1);
	auto res2 = factor(val1 * val2);
	auto res3 = factor(val1 * val3);
	// Should print
	// (100000007 2),
	// (100000007 1),(1000000007 1),
	// (2 2),(3 1),(7 1),(13 1),(41 1),(100000007 1),
	for (auto p : res1) cout << "(" << p.first << ' ' << p.second << "),"; cout << '\n';
	for (auto p : res2) cout << "(" << p.first << ' ' << p.second << "),"; cout << '\n';
	for (auto p : res3) cout << "(" << p.first << ' ' << p.second << "),"; cout << '\n';
}
