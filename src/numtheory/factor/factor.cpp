#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;
typedef long long ll;
typedef __int128 lll;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}

lll modPow(lll a, lll b, lll mod) {
	if (b & 1) return (a * modPow(a, b-1, mod)) % mod;
	if (b == 0) return 1;
	return modPow((a * a) % mod, b / 2, mod);
}
bool isWitness(lll w, lll ctz, lll odd, lll x) {
	w = modPow(w, odd, x);
	if (w == 1) return 0;
	while(ctz && (w != x - 1)) {
		w = (w * w) % x;
		--ctz;
	}
	return (ctz == 0);
}
bool isPrime(ll x) {
	if (x <= 1 || (x % 2 == 0 && x > 2)) return 0;
	ll ctz = __builtin_ctz(x-1);
	ll odd = (x-1) >> ctz;
	for (auto b : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
		ll w = b % x;
		if (w == 0) return 1;
		if (isWitness(w, ctz, odd, x)) return 0;
	}
	return 1;
}

// find 1 < x < v such that x | v. Assumes that v isn't prime
// Has the optimizations of moving only one pointer, and not taking gcd after every move
// Time Complexity: expected O(sqrt(x)) or O(v^{1 / 4})
ll pollardRho(ll v) {
	for (int c = 1;; ++c) {
		ll x = rand<ll>(0, v-1);
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
				if ((i & len) == 0) {
					x = y;
					len = 2*len+1;
				}
			}
		}
	}
}

// Returns list of primes that divide v, every prime appearing once
vector<ll> factor(ll v) {
	vector<ll> ps = {v};
	for (int i = 0; i < ps.size(); ++i) {
		if (isPrime(ps[i])) continue;
		ll div = pollardRho(ps[i]);
		ps.push_back(div);
		ps[i] /= div;
		--i;
	}
	sort(ps.begin(), ps.end());
	ps.erase(unique(ps.begin(), ps.end()), ps.end());
	return ps;
}

int main() {
	// Should print
	// 100000007 1000000007
	// 100000007 10000000007
	// 2 3 7 13 41 1000000007
	ll val1 = (ll)1e8 + 7;
	ll val2 = (ll)1e9 + 7;
	ll val3 = 2*2*2*3*3*7*13*41;
	auto res1 = factor(val1 * val1);
	auto res2 = factor(val1 * val2);
	auto res3 = factor(val1 * val3);
	for (auto p : res1) cout << p << ' '; cout << '\n';
	for (auto p : res2) cout << p << ' '; cout << '\n';
	for (auto p : res3) cout << p << ' '; cout << '\n';
}
