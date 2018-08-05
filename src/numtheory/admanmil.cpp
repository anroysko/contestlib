#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;
typedef long long ll;
typedef __int128 lll;

const ll INF = 1e18;

// Finds largest j such that vec[j] <= v.
template<class T>
int bins(const vector<T>& vec, T v) {
	int low = -1;
	int high = vec.size();
	while(true) {
		int mid = (low + high) >> 1;
		if (vec[mid] <= v) {
			low = mid;
		} else {
			high = mid - 1;
		}
	}
	return low;
}

// Given a, b, finds gcd(a, b) and x, y, such that ax + by = gcd(a, b)
// returns {gcd(a, b), {x, y}}
pair<ll, pair<lll, lll>> extEuc(ll a, ll b) {
	if (b == 0) return {a, {1, 0}};
	ll k = a / b;
	auto sub = extEuc(b, a-k*b);
	return {sub.first, {sub.second, sub.first - k*sub.second}};
}


// Given a and b, gcd(a, b) = 1, returns x such that ax = 1 (mod b)
ll modInv(ll a, ll b) {
	auto sub = extEuc(a, b); // ax + by = 1 -> ax = 1 (b)
	ll res = sub.second.first % b;
	return (res < 0 ? res + b : res);
}

// Smallest x such that v <= x^2
ll highSqrt(ll v) {
	ll low = 0;
	ll high = 1e9;
	while(low != high) {
		ll mid = (low + high);
		if (mid * mid < v) low = mid + 1;
		else high = mid;
	}
	return low;
}

struct BabyGiantStep {
	vector<pair<ll, int>> smalls;
	ll p; // Modulo (gcd(a, p) must be 1)
	ll ias; // ias = (a^smalls.size())^-1 (mod p)

	// K should be the maximum power that is the answer to any query.
	// If none is set, it is set to p-1.
	void init(lll a, ll p, ll k = -1) {
		this->p = p;
		k = (k != -1 ? k : p);

		ll s = highSqrt(k);
		smalls.resize(s);
		smalls[0] = {1, 0};
		for (int i = 1; i < s; ++i) {
			smalls[i] = {(a * smalls[i-1].first) % p, i};
		}
		ias = modInv(smalls[s-1].first, p);
		sort(smalls.begin(), smalls.end());
	}

	// If exists i <= mx such that a^i = v, returns
	// minimum such i. Otherwise returns -1.
	ll ask(ll v, ll mx = INF) {
		lll tmp = 1; // Loop large step, find matching small step
		for (ll bi = 0; bi <= mx; bi += smalls.size()) {
			ll tar = (v * tmp) % p;
			tmp = (tmp * ias) % p;
			int j = bins(smalls, {tar, smalls.size()});
			if ((j != -1) && (bi + j <= mx)) return bi + j;
		}
		return -1;
	}
}:

// Optimized version of adleman-manders-miller root extraction ( https://arxiv.org/pdf/1111.4877.pdf )
// Finds solution x to x^r = b (mod q)
// Preconditions: q > 2
ll admanmil(ll r, ll b, ll q) {
	if ((p-1) % r != 0) cout << "QAQ\n";
	// Also assume we are a residue, and that r is prime.

	//    r^t * s = rt * s = p-1
	// -> (b^(s))^(r^(t-1)) = 1 (mod p)
	ll s = q-1;
	ll t = 0;
	while(s % r == 0) {
		s /= r;
		++t;
	}
	ll rt = (q-1) / s;

	//    s | ra - 1
	// -> ra = 1 (mod s)
	// -> (b^(ra - 1))^(r^(t-1)) = 1 (mod p)
	ll a = modInv(r, s);
	if (t == 1) return modPow(b, a, q);

	// find nonresidue
	//    (roo^(s))^(r^(t-1)) != 1 (mod p)
	// -> (roo^(s))^(i * r^(t-1)) = Ki for 0 <= i < r are the r solutions to x^r = 1 (mod p)
	ll roo = 2;
	while (modPow(roo, (q-1)/r, q) != 1) roo = (random() % (q-2) + 2);
	
	// TODO rest
}


int main() {
	ll r, b, p;
	cin >> r >> b >> p;
	cout << admanmil(r, b, p) << '\n';
}
