#include <iostream>
#include <vector>
using namespace std;
using ll = long long;

// Recursive NTT
// Preconditions: P is prime, n > 1, n = 2^lg for some lg, 2^lg | P-1, 3 is a generator of P, P < 2^30
// Time Complexity: O(n log(n))
template<int P>
void ntt(vector<int>& pol, int r) {
	int h = pol.size()/2;
	vector<int> even(h);
	vector<int> odd(h);
	for (int i = 0; i < h; ++i) {
		even[i] = pol[2*i];
		odd[i] = pol[2*i+1];
	}

	if (h > 1) {
		int rr = (ll)r*r % P;
		ntt<P>(even, rr);
		ntt<P>(odd, rr);
	}

	ll mult = 1;
	for (int i = 0; i < h; ++i, mult = mult*r % P) {
		pol[i] = (even[i] + mult * odd[i]) % P;
		pol[i+h] = (even[i] - mult * odd[i]) % P;
		if (pol[i+h] < 0) pol[i+h] += P;
	}
}

constexpr ll modPow(ll a, ll b, ll c) {
	if ((b & 1) == 1) return a * modPow(a, b-1, c) % c;
	if (b == 0) return 1 % c;
	return modPow(a*a % c, b / 2, c);
}

// Calculates a * b = c (mod P) for two polynomials a and b. Assumes a[i], b[i] >= 0.
// Time Complexity: O(n log n) where n = O(a.size() + b.size())
template<int P>
vector<int> polyMult(vector<int>& a, vector<int>& b) {
	int as = a.size();
	int bs = b.size();
	int n = 1;
	while(n < (as + bs)) n <<= 1;
	int r = modPow(3, (P-1)/n, P);
	int rev_r = modPow(r, P-2, P);
	int rev_n = modPow(n, P-2, P);

	vector<int> ap (n, 0);
	vector<int> bp (n, 0);
	for (int i = 0; i < as; ++i) ap[i] = a[i] % P;
	for (int i = 0; i < bs; ++i) bp[i] = b[i] % P;
	
	ntt<P>(ap, r);
	ntt<P>(bp, r);

	vector<int> cp (n);
	for (int i = 0; i < n; ++i) cp[i] = ((ll)ap[i] * bp[i]) % P;

	ntt<P>(cp, rev_r);
	
	vector<int> res(as + bs - 1);
	for (int i = 0; i < res.size(); ++i) res[i] = ((ll)cp[i] * rev_n) % P;
	return res;
}

// Three primes p all of which have 2^21 | p-1. 3 is a generator for all of them.
constexpr int ntt_primes[3] = {998244353, 1004535809, 985661441};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	
	int as, bs;
	cin >> as >> bs;
	vector<int> a(as);
	vector<int> b(bs);
	for (int i = 0; i < as; ++i) cin >> a[i];
	for (int i = 0; i < bs; ++i) cin >> b[i];
	
	vector<int> ab = polyMult<ntt_primes[0]>(a, b);
	for (int i = 0; i < ab.size(); ++i) cout << ab[i] << ' '; cout << '\n';
}
