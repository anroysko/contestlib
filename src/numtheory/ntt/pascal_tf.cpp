#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;
using ll = long long;

template<int P>
ll modPow(ll a, ll b) {
	if ((b & 1) == 1) return a * modPow<P>(a, b-1) % P;
	if (b == 0) return 1 % P;
	return modPow<P>(a*a % P, b / 2);
}

// Applies the bit-reverse permutation to vec.
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
// Preconditions: P is prime, n = 2^lg for some lg, 2^lg | P-1, 3 is a generator of P, P < 2^30
// Time Complexity: O(n log(n))
template<int P>
void ntt(vector<int>& pol, bool inv) {
	bitReverse(pol); // Lets us calculate FFT iteratively

	int n = pol.size();
	int lg = 0;
	while((1<<lg) < n) ++lg;
	
	ll nth_root = modPow<P>(3, (P-1)/n);
	if (inv) nth_root = modPow<P>(nth_root, n-1);

	vector<ll> pows(lg, nth_root); // nth_root^(2^(lg-1-i))
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

// Calculates a * b = c (mod P) for two polynomials a and b. Assumes 0 <= a[i], b[i] < P.
// Time Complexity: O(n log n) where n = O(a.size() + b.size())
template<int P>
vector<int> polyMult(vector<int> a, vector<int> b) {
	int as = a.size();
	int bs = b.size();
	int n = 1;
	while(n < (as + bs)) n <<= 1;

	a.resize(n, 0);
	b.resize(n, 0);
	ntt<P>(a, false);
	ntt<P>(b, false);

	vector<int> c(n);
	int n_inv = modPow<P>(n, P-2);
	for (int i = 0; i < n; ++i) {
		ll prod = (ll)a[i]*b[i] % P;
		c[i] = prod*n_inv % P;
	}

	ntt<P>(c, true);
	c.resize(as+bs-1);

	return c;
}

// Multiplies the input vector with the given hankel matrix.
template<int P>
vector<int> hankelMult(vector<int> res, vector<int> hankel) {
	int n = res.size();
	reverse(hankel.begin(), hankel.end());
	vector<int> mid = polyMult<P>(res, hankel);
	for (int i = 0; i < n; ++i) res[i] = mid[2*n-2-i];

	return res;
}

// Struct for precalculating factorials
template<int P>
struct FactPrecalc {
	vector<int> fact;
	vector<int> inv_fact;
	
	FactPrecalc(int n) : fact(n+1, 1), inv_fact(n+1, 1) {
		for (int i = 2; i <= n; ++i) {
			fact[i] = (ll)fact[i-1] * i % P;
			inv_fact[i] = modPow<P>(fact[i], P-2);
		}
	}
	int choose(int a, int b) const {
		if (a < b) return 0;
		ll div = (ll)inv_fact[a-b] * inv_fact[b] % P;
		return fact[a] * div % P;
	}
};

// Given the number of ways to reach the left side of a rectangle, returns the number of ways
// to go through the left side to the top side, and to go through the left side to the right side.
template<int P>
pair<vector<int>, vector<int>> countPaths(vector<int> a, int w, const FactPrecalc<P>& fp) {
	int h = a.size();
	vector<int> hankel_1(2*h-1);
	for (int i = 0; i < 2*h-1; ++i) hankel_1[i] = fp.choose(w-h+i, w-1);
	vector<int> res_c = hankelMult<P>(a, hankel_1);

	int n = max(w, h);
	for (int i = 0; i < h; ++i) a[i] = a[i] * (ll)fp.inv_fact[i] % P;
	if (a.size() < n) a.resize(n, 0);

	vector<int> hankel_2(2*n-1);
	for (int i = 0; i < 2*n-1; ++i) hankel_2[i] = fp.fact[i];
	vector<int> res_d = hankelMult<P>(a, hankel_2);

	res_d.resize(w);
	for (int i = 0; i < w; ++i) res_d[i] = res_d[i] * (ll)fp.inv_fact[i] % P;

	return {res_c, res_d};
}
template<int P>
pair<vector<int>, vector<int>> solveRect(const vector<int>& a, const vector<int>& b, const FactPrecalc<P>& fp) {
	int h = a.size();
	int w = b.size();

	vector<int> res_c1, res_c2, res_d1, res_d2;
	tie(res_c1, res_d1) = countPaths<P>(a, w, fp);
	tie(res_d2, res_c2) = countPaths<P>(b, h, fp);

	vector<int> c(h);
	for (int i = 0; i < h; ++i) c[i] = (res_c1[i] + res_c2[i]) % P;
	vector<int> d(w);
	for (int i = 0; i < w; ++i) d[i] = (res_d1[i] + res_d2[i]) % P;

	return {c, d};
}

// Three primes p all of which have 2^21 | p-1. 3 is a generator for all of them.
constexpr int ntt_primes[3] = {998244353, 1004535809, 985661441};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	
	int h, w;
	cin >> h >> w;
	int n = max(h, w);
	FactPrecalc<ntt_primes[0]> fp(2*n-1);

	vector<int> a(h);
	vector<int> b(w);
	for (int i = 0; i < h; ++i) cin >> a[i];
	for (int i = 0; i < w; ++i) cin >> b[i];
	vector<int> c, d;
	tie(c, d) = solveRect<ntt_primes[0]>(a, b, fp);

	// Visualize the results
	// The output format of solveRect is a bit odd, and it sort of reverses a and b, so be careful!
	cout << "Visualization of answer:\n";
	int hh = h + 2;
	int ww = w + 2;
	vector<int> disp(ww * hh, -1);
	for (int x = 1; x <= w; ++x) {
		disp[x + 0*ww] = d[x-1];
		disp[x + (hh-1)*ww] = b[w-x];
	}
	for (int y = 1; y <= h; ++y) {
		disp[0 + y*ww] = a[y-1];
		disp[ww-1 + y*ww] = c[h-y];
	}
	for (int y = 0; y < hh; ++y) {
		for (int x = 0; x < ww; ++x) {
			if (disp[x+y*ww] == -1) cout << setw(4) << ".";
			else cout << setw(4) << disp[x+y*ww];
		}
		cout << '\n';
	}
	cout << "Filled in:\n";
	for (int x = 1; x <= w; ++x) {
		for (int y = h; y >= 1; --y) {
			disp[x+y*ww] = (disp[x-1+y*ww] + disp[x+(y+1)*ww]) % ntt_primes[0];
		}
	}
	for (int y = 0; y < hh; ++y) {
		for (int x = 0; x < ww; ++x) {
			if (disp[x+y*ww] == -1) cout << setw(4) << ".";
			else cout << setw(4) << disp[x+y*ww];
		}
		cout << '\n';
	}
}
