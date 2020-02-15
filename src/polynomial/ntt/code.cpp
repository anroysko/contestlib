// Wrapped to a single line for constexpr support
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
		ll mult = modPow(root, n / (2*h), P);
		for (int j = 0; j < n; j += 2*h) {
			ll curr = 1;
			for (int i = j; i < j+h; ++i) {
				ll v = (curr * vec[i + h]) % P;
				curr = (curr * mult) % P;
				vec[i+h] = mSub(vec[i], v, P);
				vec[i] = mAdd(vec[i], v, P);
			}
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

// parallelised Garner's algorithm https://cp-algorithms.com/algebra/chinese-remainder-theorem.html
// complexity: O(n |p|^2), where n is the size of the vectors in y
vector<int> crt(vector<vector<int>> y, const vector<int>& p, int m) {
	for (int i = 0; i < p.size(); ++i) {
		for (int j = 0; j < i; ++j) {
			ll inv = modPow(p[j], p[i]-2, p[i]);
			for (int k = 0; k < y[i].size(); ++k) {
				y[i][k] = ((ll)y[i][k] - y[j][k]) * inv % p[i];
				if (y[i][k] < 0) y[i][k] += p[i];
			}
		}
	}
	vector<int> res(y[0].size(), 0);
	for (int i = 0, mult = 1; i < p.size(); ++i) {
		for (int k = 0; k < y[i].size(); ++k) {
			res[k] = (res[k] + (ll)mult * y[i][k]) % m;
		}
		mult = ((ll)mult * p[i]) % m;
	}
	return res;
}

// Does polymult for arbitrary modulo. Assumes 0 <= a[i], b[i] < p <= NTTP[0]
vector<int> polyMultMod(const vector<int>& a, const vector<int>& b, int p) {
	// Primes p for all of which 3 is a generator and 2^20 | p-1.
	constexpr int NTTP[3] = {1004535809, 1092616193, 1161822209};

	vector<vector<int>> vecs(3);
	vecs[0] = polyMult<NTTP[0]>(a, b);
	vecs[1] = polyMult<NTTP[1]>(a, b);
	vecs[2] = polyMult<NTTP[2]>(a, b);
	return crt(vecs, {NTTP[0], NTTP[1], NTTP[2]}, p);
}
