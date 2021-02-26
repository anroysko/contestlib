#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll P = 998244353; // Nice prime for NTT

constexpr ll mAdd(ll a, ll b, ll c) {
	return (a+b >= c ? a+b-c : a+b);
}
constexpr ll mSub(ll a, ll b, ll c) {
	return (a < b ? a-b+c : a-b);
}
constexpr ll modPow(ll a, ll b, ll c) {
	return ((b & 1) ? (a * modPow(a, b ^ 1, c) % c) : (b == 0) ? 1 : (modPow(a*a % c, b >> 1, c)));
}

// Iterative NTT in O(n log n)
// Preconditions: P is prime, n = 2^m, 2^m | P-1, 3 is a generator of P
template<int P>
void ntt(vector<int>& vec, bool inv) {
	// Apply the bit-reverse permutation
	int n = vec.size();
	for (int i = 0; i < n; ++i) {
		int t = 0;
		for (int j = 0; (1<<j) < n; ++j) {
			if (i & (1<<j)) t |= n >> (j+1);
		}
		if (i < t) swap(vec[i], vec[t]);
	}

	// Do Cooley-Tukey
	int root = modPow(3, (inv ? (n-1) : 1) * (ll)(P-1) / n, P);
	for (int h = 1; h < n; h <<= 1) {
		ll curr = 1, mult = modPow(root, n / (2*h), P);
		for (int j = 0; j < h; ++j) {
			for (int i = j; i < n; i += 2*h) {
				ll v = (curr * vec[i^h]) % P;
				vec[i^h] = mSub(vec[i], v, P);
				vec[i] = mAdd(vec[i], v, P);
			}
			curr = (curr * mult) % P;
		}
	}
}

// Calculates a * b = c (mod P) for two polynomials a and b. P must be valid for ntt. Assumes 0 <= a[i], b[i] < P.
template<int P>
vector<int> polyMult(vector<int> a, vector<int> b) {
	int n = 1, t = (int)a.size() + b.size() - 1;
	while(n < t) n <<= 1;

	a.resize(n, 0); b.resize(n, 0);
	ntt<P>(a, 0); ntt<P>(b, 0);

	vector<int> c(n);
	int n_inv = modPow(n, P-2, P);
	for (int i = 0; i < n; ++i) c[i] = (ll)a[i]*b[i] % P * n_inv % P;

	ntt<P>(c, 1);
	c.resize(t);
	return c;
}


// Finds the polynomial inverse pol * inv = 1 (mod x^n) in O(n log n) assuming pol[0] = 1
// Uses Hensel lifting: given pol * inv = 1 (mod x^h), find pol * inv' = 1 (mod x^2h)
// http://people.csail.mit.edu/madhu/ST12/scribe/lect06.pdf
template<int P>
vector<int> inverse(const vector<int>& pol, int n) {
	vector<int> inv = {1}, cur = {1};
	for (int h = 1; h < n; h <<= 1) {
		cur.resize(2*h, 0);
		for (int i = h; i < 2*h && i < pol.size(); ++i) cur[i] = pol[i];
		vector<int> prod = polyMult<P>(cur, inv);

		vector<int> high(h);
		for (int i = 0; i < h; ++i) high[i] = prod[i+h];
		high = polyMult<P>(high, inv);

		inv.resize(2*h);
		for (int i = 0; i < h; ++i) inv[i+h] = mSub(0, high[i], P);
	}
	inv.resize(n);
	return inv;
}

// Finds a such that p = aq + b for some b, deg(b) < deg(q). q must be monic (highest degree term has multiplier 1)
template<int P>
vector<int> polyDiv(vector<int> p, vector<int> q) {
	int deg = (int)p.size() - (int)q.size();
	reverse(p.begin(), p.end());
	reverse(q.begin(), q.end());
	vector<int> rqi = inverse<P>(q, deg + 1);
	vector<int> ra = polyMult<P>(rqi, p);
	ra.resize(deg + 1);
	reverse(ra.begin(), ra.end());
	return ra;
}

template<int P>
vector<int> polyMod(const vector<int>& p, const vector<int>& q) {
	if (q.size() > p.size()) return p;
	vector<int> div = polyDiv<P>(p, q);
	vector<int> prod = polyMult<P>(q, div);
	vector<int> rem = p;
	for (int i = 0; i < p.size(); ++i) rem[i] = mSub(rem[i], prod[i], P);
	while(rem.size() > 1 && rem.back() == 0) rem.pop_back();
	return rem;
}

// Evaluates a degree n polynomial at n points in O(n log^2 n)
// Follows from the identity P(x) mod (x - x0) = P(x0) and O(n log n) polynomial division
// degree 10^5 polynomial and 10^5 points in ~4s
template<int P>
vector<int> multiPointEval(const vector<int>& pol, const vector<int>& xs) {
	int n = xs.size();
	vector<vector<int>> tree(2*n);
	for (int i = n; i < 2*n; ++i) tree[i] = {(int)mSub(0, xs[i-n], P), 1};
	for (int i = n-1; i > 0; --i) tree[i] = polyMult<P>(tree[2*i], tree[2*i+1]);

	// This section should be the bottleneck
	// 2^k modulo operations on polynomials of size n / 2^k
	// -> need fast polynomial modulo both for small and large polynomials!
	vector<vector<int>> rem(2*n);
	rem[1] = polyMod<P>(pol, tree[1]);
	for (int i = 2; i < 2*n; ++i) {
		rem[i] = polyMod<P>(rem[i >> 1], tree[i]);
	}

	vector<int> res(n);
	for (int i = n; i < 2*n; ++i) res[i-n] = rem[i][0];
	return res;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n;
	cin >> n;

	vector<int> pol(n);
	for (int& a : pol) cin >> a;

	vector<int> xs(n);
	for (int& x : xs) cin >> x;

	vector<int> vals = multiPointEval<P>(pol, xs);
	for (auto v : vals) cout << v << ' '; cout << '\n';
}
