#include <iostream>
#include <vector>
#include <random>
#include <utility>
using namespace std;
typedef long long ll;
typedef __int128 lll;

void sieve(int n, vector<int>& primes, vector<int>& div_ind) {
	primes.reserve(n+1);
	div_ind.resize(n+1);
	for (int i = 0; i <= n; ++i) div_ind[i] = -1;
	for (int i = 2; i <= n; ++i) {
		if (div_ind[i] == -1) {
			div_ind[i] = primes.size();
			primes.push_back(i);
		}
		for (int j = 0; j <= div_ind[i]; ++j) {
			int t = primes[j] * i;
			if (t > n) break;
			div_ind[t] = j;
		}
	}
}


lll modPow(lll w, lll u, lll val) {
	if (u & 1) return (w * modPow(w, u-1, val)) % val;
	if (u == 0) return 1;
	lll sub = modPow(w, u>>1, val);
	return (sub * sub) % val;
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

// Calculates largest x such that x*x <= v
// Time Complexity: O(log(v))
// Preconditions: x < 2^31
ll lowerSqrt(ll v) {
	ll x = 0;
	ll xx = 0;
	for (int j = 30; j >= 0; --j) {
		ll nxx = (xx) + (x<<(j+1)) + ((ll)1 << (2*j));
		if (nxx <= v) {
			xx = nxx;
			x |= (1<<j);
		}
	}
	return x;
}

ll gcd(ll a, ll b) {
	if (b == 0) return a;
	return gcd(b, a % b);
}

// find x such that x | v.
// Time Complexity: expected O(sqrt(v) * log(v))
// Preconditions: v % 2 != 0, there is no x such that x^k = v for k > 1, v is not prime
ll pollardRho(ll v) {
	for (int c = 1;; ++c) {
		ll x = 1;
		ll y = ((lll)x * x + 1);
		while(true) {
			ll g = gcd(abs(x-y), v);
			if (g != 1) {
				if (g == v) break;
				else return g;
			}
			x = ((lll)x * x + c) % v;
			y = ((lll)y * y + c) % v;
			y = ((lll)y * y + c) % v;
		}
	}
}

// Factor v into pairs pi, xi, such that v = \prod pi^xi
// Time Complexity: O(cuberoot(v))
// Preconditions: primes must contain all primes <= cuberoot(v)
// Returns: vector of pairs {pi, xi}
vector<pair<ll, int>> factor(ll v, const vector<int>& primes) {
	vector<pair<ll, int>> res;
	for (int j = 0; j < primes.size(); ++j) {
		int cou = 0;
		while(true) {
			ll rem = v / primes[j];
			if (rem * primes[j] == v) {
				++cou;
				v = rem;
			} else {
				break;
			}
		}
		if (cou) res.push_back({primes[j], cou});
	}

	if (v != 1) {
		if (isPrime(v)) {
			res.push_back({v, 1});
		} else {
			ll ls = lowerSqrt(v);
			if (ls * ls == v) {
				res.push_back({ls, 2});
			} else {
				ll p = pollardRho(v);
				ll q = v / p;
				res.push_back({min(p, q), 1});
				res.push_back({max(p, q), 1});
			}
		}
	}
	return res;
}

int main() {
	const int N = 10 + (int)1e6;
	vector<int> primes;
	vector<int> div_ind;
	sieve(N, primes, div_ind);

	ll val1 = (ll)1e8 + 7;
	ll val2 = (ll)1e9 + 7;
	ll val3 = 2*2*3*7*13*41;
	auto res1 = factor(val1 * val1, primes);
	auto res2 = factor(val1 * val2, primes);
	auto res3 = factor(val1 * val3, primes);
	// Should print
	// (100000007 2),
	// (100000007 1),(1000000007 1),
	// (2 2),(3 1),(7 1),(13 1),(41 1),(100000007 1),
	for (auto p : res1) cout << "(" << p.first << ' ' << p.second << "),"; cout << '\n';
	for (auto p : res2) cout << "(" << p.first << ' ' << p.second << "),"; cout << '\n';
	for (auto p : res3) cout << "(" << p.first << ' ' << p.second << "),"; cout << '\n';
}
