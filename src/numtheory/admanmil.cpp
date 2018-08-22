#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
using namespace std;
typedef long long ll;
typedef __int128 lll;

const ll INF = 1e18;

// Finds largest j such that vec[j] <= v.
template<class T>
int bins(const vector<T>& vec, T v) {
	int low = -1;
	int high = (int)vec.size()-1;
	while(low < high) {
		int mid = (low + high + 1) >> 1;
		if (vec[mid] <= v) {
			low = mid;
		} else {
			high = mid - 1;
		}
	}
	return low;
}

// Calculates a^b mod c in log(b) time.
ll modPow(ll a, ll b, ll c) {
	if (b & 1) return ((lll)a * modPow(a, b^1, c)) % c;
	if (b == 0) return 1 % c;
	return modPow(((lll)a*a)%c, b>>1, c);
}

// returns {gcd(a, b), {x, y}} such that ax + by = gcd(a, b)
pair<ll, pair<lll, lll>> extEuc(ll a, ll b) {
	if (b == 0) return {a, {1, 0}};
	ll k = a / b;
	auto sub = extEuc(b, a-k*b);
	return {sub.first, {sub.second.second, sub.second.first - k*sub.second.second}};
}

// returns x such that ax = 1 (mod b). Requires gcd(a, b) = 1.
ll modInv(ll a, ll b) {
	if (b == 1) return 1; // Annoying special case needed for admanmil
	auto sub = extEuc(a, b); // ax + by = 1 -> ax = 1 (b)
	ll res = sub.second.first % b;
	return (res < 0 ? res + b : res);
}

// Baby-step Giant-step algorithm for discrete logarithm ( https://en.wikipedia.org/wiki/Baby-step_giant-step )
struct BabyGiantStep {
	vector<ll> vals;
	vector<int> inds;
	ll p, ias;

	// requires gcd(a, p) = 1. k should be sqrt(order(a))
	// Complexity: O(k log(k)). Memory usage is O(k).
	void init(ll a, ll p, ll k) {
		this->p = p;

		lll mult = 1;
		vector<pair<ll, int>> tmp (k);
		for (int i = 0; i < k; ++i) {
			tmp[i] = {mult, i};
			mult = (mult * a) % p;
		}
		sort(tmp.begin(), tmp.end());

		vals.resize(k);
		inds.resize(k);
		for (int i = 0; i < k; ++i) vals[i] = tmp[i].first;
		for (int i = 0; i < k; ++i) inds[i] = tmp[i].second;
		ias = modInv(mult, p);
	}

	// If exists i <= mx such that a^i = v, returns minimum such i. Otherwise returns -1.
	// Complexity: O(max(mx, i) log(k) / k) where i is the found index.
	ll ask(ll b, ll mx = INF) {
		for (ll bi = 0; bi <= mx;) {
			int j = bins(vals, b);
			if ((j != -1) && (vals[j] == b)) {
				ll res = bi + inds[j];
				if (res <= mx) return res;
			}
			b = ((lll)b * ias) % p; // Giant step
			bi += vals.size(); // Update base ind
		}
		return -1;
	}
};

// Optimized version of adleman-manders-miller root extraction ( https://arxiv.org/pdf/1111.4877.pdf )
// Finds solution x to x^r = b (mod q). If none exists, returns -1.
// Preconditions: r and q are prime
// Complexity: O(log(q) min(q^(1/4), log_r(q) sqrt(r))
ll admanmil(ll r, ll b, ll q) {
	if (q == 2) return b; // q-1 = 1 is annoying to deal with.

	//    r^t * s = p-1
	// -> (b^(s))^(r^(t-1)) = 1 (mod p)
	ll s = q-1;
	ll t = 0;
	while(s % r == 0) {
		s /= r;
		++t;
	}
	if (t == 0) return modPow(b, modInv(r, q-1), q); // Trivial case 1
	if (modPow(b, (q-1)/r, q) != 1) return -1; // nonresidue

	//    s | ra - 1
	// -> ra = 1 (mod s)
	// -> (b^(ra - 1))^(r^(t-1)) = 1 (mod p)
	ll a = modInv(r, s);
	if (t == 1) return modPow(b, a, q);

	// find nonresidue
	//    (roo^(s))^(r^(t-1)) != 1 (mod p)
	// -> (roo^(s))^(i * r^(t-1)) = Ki for 0 <= i < r are the r solutions to x^r = 1 (mod p)
	ll roo = 2;
	while (modPow(roo, (q-1)/r, q) == 1) roo = (random() % (q-2) + 2);

	BabyGiantStep ks; // For finding the correct Ki
	ks.init(modPow(roo, (q-1)/r, q), q, sqrt(r)); // O(log(r) sqrt(r))

	// Invariant res^r = base * b
	// Invariant base^(r^(t-i+1)) = 1
	// -> Loop runs at most t-1 steps.
	ll res = modPow(b, a, q);
	ll base = modPow(b, r*a-1, q);
	ll rp = 1;
	for (int i = 2; base != 1; ++i) {
		ll val = modPow(base, modPow(r, t-i, q-1), q);
		ll j = (r - ks.ask(val)) % r; // O(log(r) sqrt(r))
		//    base^(r^(t-i)) * (roo^(s*j))^(r^(t-1)) = 1
		// -> (base * roo^(s*j*r^(i-1)))^(r^(t-i)) = 1
		
		// set base <- base * roo^(s*j * r^(i-1)) and
		//     res  <- res  * roo^(s*j * r^(i-2))
		// maintaining the invariant

		ll pw = (s*j) % (q-1);
		res = (lll)res * modPow(roo, (pw*rp) % (q-1), q) % q;
		rp = (rp * r) % (q-1);
		base = (lll)base * modPow(roo, (pw*rp) % (q-1), q) % q;
	}
	//    base = 1
	// -> res^r = b
	return res;
}


int main() {
	ll r, b, p;
	cin >> r >> b >> p;
	ll res = admanmil(r, b, p);
	cout << res << '\n';
}
