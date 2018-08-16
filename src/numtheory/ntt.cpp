#include <iostream>
#include <vector>
using namespace std;
typedef __int128 lll;
typedef long long ll;

// Three primes p all of which have 2^21 | p-1. 998244353 is the one most commonly seen in NTT-problems
// Additionally, 3 is a generator for all of them.
// To calculate in arbitrary modulo, first calculate in 1-3 moduloes, and then use CRT (chinese remainder theorem).
// If inputs are mod-p, and the array is of size n, the output integers are at most np^2, so for
// moduloes <= 1e9 three integers should always be enough.
constexpr int ntt_primes[3] = {998244353, 1004535809, 985661441};

ll modPow(ll a, ll b, ll c) {
	if ((b & 1) == 1) return a * modPow(a, b-1, c) % c;
	if (b == 0) return 1 % c;
	ll sub = modPow(a, b / 2, c);
	return sub * sub % c;
}

// Applies the bit-reverse permutation to the given vector
// Preconditions: vec.size() = 2^lg for some lg.
// Time Complexity: O(n log n)
template<class T>
void bitReverse(vector<T>& vec) {
	int n = vec.size();
	int lg = 0;
	while((1<<lg) < n) ++lg;
	for (int i = 0; i < n; ++i) {
		int t = 0;
		for (int j = 0; j < lg; ++j) {
			if (i & (1<<j)) t |= 1<<(lg-1-j);
		}
		if (i < t) swap(vec[i], vec[t]);
	}
}


// Iterative FFT.
// Preconditions: P is prime, n | P-1, n = 2^lg for some lg, 2 is a generator of P, P < 2^30
// Time Complexity: O(n log(n))
template<int P>
void ntt(vector<int>& pol, bool inv) {
	bitReverse(pol); // Lets us calculate FFT iteratively

	int n = pol.size();
	int lg = 0;
	while((1<<lg) < n) ++lg;
	
	ll nth_root = modPow(3, (P-1)/n, P);
	if (inv) nth_root = modPow(nth_root, n-1, P);

	vector<ll> pows(lg); // nth_root^(2^(lg-1-i))
	pows.back() = nth_root;
	for (int i = lg-2; i >= 0; --i) pows[i] = (pows[i+1] * pows[i+1]) % P;

	ll tmp = 1;
	for (int i = 0; i < n; ++i) tmp = (tmp * nth_root) % P;	

	for (lg = 0; (1<<lg) < n; ++lg) {
		int h = 1<<lg;	// half
		int len = h<<1;	// step length
		ll root = pows[lg];
		for (int j = 0; j < n; j += len) {
			ll curr = 1;
			for (int i = j; i < j+h; ++i) {
				int tmp = (curr * pol[i + h]) % P;
				curr = (curr * root) % P;
				
				pol[i + h] = P - tmp + pol[i];
				if (pol[i + h] >= P) pol[i + h] -= P;
				
				pol[i] += tmp;
				if (pol[i] >= P) pol[i] -= P;
			}
		}
	}
}

// Calculates a * b = c (mod P) for two polynomials a and b.
// Time Complexity: O(n log n) where n = O(max(a.size(), b.size()))
template<int P>
vector<int> polyMult(vector<int>& a, vector<int>& b) {
	int as = a.size();
	int bs = b.size();
	int n = 1;
	while(n < (as + bs)) n <<= 1;

	vector<int> ap (n, 0);
	vector<int> bp (n, 0);
	
	for (int i = 0; i < as; ++i) ap[i] = (P + a[i]) % P;
	for (int i = 0; i < bs; ++i) bp[i] = (P + b[i]) % P;
	
	ntt<P>(ap, false);
	ntt<P>(bp, false);

	vector<int> cp (n);
	for (int i = 0; i < n; ++i) cp[i] = ((ll)ap[i] * bp[i]) % P;

	ntt<P>(cp, true);
	
	int n_inv = modPow(n, P-2, P);
	vector<int> res(as + bs - 1);
	for (int i = 0; i < res.size(); ++i) res[i] = ((ll)cp[i] * n_inv) % P;
	return res;
}

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
	
	/*
	vector<int> ans (as+bs-1, 0);
	for (int i = 0; i < as; ++i) {
		for (int j = 0; j < bs; ++j) {
			ans[i+j] += (ll)a[i] * b[j] % ntt_primes[0];
		}
	}
	for (int i = 0; i < ans.size(); ++i) cout << ans[i] << ' '; cout << '\n';
	*/
}
