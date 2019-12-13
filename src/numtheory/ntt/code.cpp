// Wrapped to a single line for constexpr support
constexpr ll modPow(ll a, ll b, ll c) {
	return ((b & 1) ? (a * modPow(a, b ^ 1, c) % c) : (b == 0) ? 1 : (modPow(a*a % c, b >> 1, c)));
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

// Iterative FFT in O(n log n)
// Preconditions: P is prime, n = 2^m, 2^m | P-1, 3 is a generator of P, P < 2^30
template<int P>
void ntt(vector<int>& pol, bool inv) {
	int n = pol.size();
	bitReverse(pol);
	int root = modPow(3, (P-1) / n, P);
	if (inv) root = modPow(root, n-1, P);

	for (int m = 0; (1<<m) < n; ++m) {
		int h = 1<<m;	// half of step length
		int s = h<<1;	// step length
		ll mult = modPow(root, n >> m, P);
		for (int j = 0; j < n; j += s) {
			ll curr = 1;
			for (int i = j; i < j+h; ++i) {
				int tmp = (curr * pol[i + h]) % P;
				pol[i + h] = P - tmp + pol[i];
				pol[i] += tmp;
				if (pol[i + h] >= P) pol[i + h] -= P;
				if (pol[i] >= P) pol[i] -= P;
				curr = (curr * mult) % P;
			}
		}
	}
}

// Calculates a * b = c (mod P) for two polynomials a and b. P must be valid for ntt. Assumes 0 <= a[i], b[i] < P.
template<int P>
vector<int> polyMult(vector<int> a, vector<int> b) {
	int n = 1, m = (int)a.size() + b.size() - 1;
	while(n <= m) n <<= 1;

	a.resize(n, 0); b.resize(n, 0);
	ntt<P>(a, 0); ntt<P>(b, 0);

	vector<int> c(n);
	int n_inv = modPow(n, P-2, P);
	for (int i = 0; i < n; ++i) {
		ll prod = (ll)a[i]*b[i] % P;
		c[i] = prod*n_inv % P;
	}

	ntt<P>(c, 1); c.resize(m);
	return c;
}


// parallelised Garner's algorithm https://cp-algorithms.com/algebra/chinese-remainder-theorem.html
// complexity: O(n |p|^2), where n is the size of the vectors in y
vector<int> crt(const vector<vector<int>>& y, const vector<int>& p, int m) {
	// a = x[0] + x[1] p[0] + ... + x[n-1] p[0] ... p[n-2]
	// we know a = y[i] (mod p[i])
	// -> x[i] = ( ... ((y[i] - x[0]) * p[0]^{-1} - x[1]) * ...) * p[i-1]^{-1} (mod p[i])
	// At i'th iteration of the loop we replace y[i] with x[i]
	for (int i = 0; i < p.size(); ++i) {
		for (int x = 0; x < i; ++x) {
			ll inv = modPow(p[x], p[i]-2, p[i]);
			for (int j = 0; j < y[i].size(); ++j) {
				y[i][j] = (y[i][j] - y[x][j]) * inv % p[i];
			}
		}
	}
	vector<int> res(y[0].size(), 0);
	for (int i = 0, mult = 1; i < p.size(); ++i) {
		for (int j = 0; j < vecs[i].size(); ++j) {
			res[j] = (res[j] + (ll)mult * x[i][j]) % m;
			if (res[j] < 0) res[j] += m;
		}
		mult = ((ll)mult * p[i]) % m;
	}
	return res;
}

// Primes p for all of which 3 is a generator and 2^20 | p-1.
constexpr int NTTP[5] = {1004535809, 1092616193, 1161822209, 998244353, 985661441};

// Does polymult for arbitrary modulo that is at most 10^9 + 7
vector<int> polyMultMod(const vector<int>& a, const vector<int>& b, int p) {
	vector<vector<int>> vecs(3);
	vecs[0] = polyMult<NTTP[0]>(a, b);
	vecs[1] = polyMult<NTTP[1]>(a, b);
	vecs[2] = polyMult<NTTP[2]>(a, b);
	return crt(vecs, {NTTP[0], NTTP[1], NTTP[2]}, p);
}
