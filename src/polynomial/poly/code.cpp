
constexpr ll mAdd(ll a, ll b, ll c) {
	return (a+b >= c ? a+b-c : a+b);
}
constexpr ll mSub(ll a, ll b, ll c) {
	return (a < b ? a-b+c : a-b);
}
constexpr ll modPow(ll a, ll b, ll c) {
	return ((b & 1) ? (a * modPow(a, b ^ 1, c) % c) : (b == 0) ? 1 : (modPow(a*a % c, b >> 1, c)));
}

// Primes suitable for NTT
constexpr int NTTP[4] = {998244353, 1004535809, 1092616193, 1161822209};

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

// Garner's algorithm for chinese remainder theorem in O(|P|^2)
// https://cp-algorithms.com/algebra/chinese-remainder-theorem.html
class Garner {
		vector<int> p;
		vector<vector<int>> inv;
	public:
		Garner(const vector<int>& primes) : p(primes), inv(primes.size()) {
			for (int i = 0; i < p.size(); ++i) {
				for (int j = 0; j < i; ++j) inv[i].push_back(modPow(p[j] % p[i], p[i] - 2, p[i]));
			}
		}
		ll eval(vector<int> r, int mod) {
			ll res = 1, mult = 1;
			for (int i = 0; i < p.size(); ++i) {
				for (int j = 0; j < i; ++j) {
					r[i] = ((ll)r[i] - r[j]) * inv[i][j] % p[i];
				}
				res = (res + mult * r[i]) % mod;
				mult *= p[i];
			}
			return (res < 0 ? res + mod : res);
		}
};


template<int P>
class Poly {
	private:
		vector<int> co; // coefficients

	public:
		Poly(vector<int> coeff) : co(coeff) {}

		int deg() const { return (int)co.size() - 1; }
		int& operator[](int i) { return co[i]; }
		vector<int>& data() { return co; }

		// Evaluate
		int operator()(int x) const {
			ll res = 0;
			for (int i = (int)co.size() - 1; i >= 0; --i) {
				res = (x * res + co[i]) % P;
			}
			return res;
		}		

		Poly operator+(const Poly<P>& rhs) const {
			vector<int> res(max(co.size(), rhs.size()));
			for (int i = 0; i < res.size(); ++i) {
				res[i] = modAdd((i < co.size() ? co[i] : 0), (i < rhs.co.size() ? rhs.co[i] : 0), P);
			}
			return Poly<P>(res);
		}
		Poly operator-(const Poly<P>& rhs) const {
			vector<int> res(max(co.size(), rhs.size()));
			for (int i = 0; i < res.size(); ++i) {
				res[i] = modSub((i < co.size() ? co[i] : 0), (i < rhs.co.size() ? rhs.co[i] : 0), P);
			}
			return Poly<P>(res);
		}
		Poly operator*(const Poly<P>& rhs) const {
			if constexpr(P in ) {
				return Poly<P>(res);
			} else {

			}
		}
		Poly operator/(const Poly<P>& rhs) const {
			// TODO
		}
		Poly operator%(const Poly<P>& rhs) const {
			// TODO
		}
		vector<> factor() const {

		}

		static pair<Poly, Poly> divWithRem(const Poly<P>& a, Poly<P>& b) const {
			// TODO
		}
		static Poly gcd(Poly& le, Poly& ri) {
			
		}
};
