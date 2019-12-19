ll modPow(ll a, ll b) {
	if (b & 1) return a * modPow(a, b^1) % MOD;
	if (b == 0) return 1;
	return modPow(a*a % MOD, b >> 1);
}

// Evaluates the polynomial at the given point
ll eval(const vector<ll>& pol, ll x) {
	ll res = 0;
	for (int i = (int)pol.size() - 1; i >= 0; --i) {
		res = (pol[i] + x * res) % MOD;
	}
	if (res < 0) res += MOD;
	return res;
}

// Given P_{k}(x) finds P_{k+1}(x) = P_{k}(x) (-x(k+1)) + P'_{k}(x) (x^2 - x)
vector<ll> nxt(const vector<ll>& pol) {
	int k = (int)pol.size() - 1;
	vector<ll> res(k+2, 0);
	for (int j = k+1; j > 0; --j) {
		ll c = 0; // Coefficient for x^j in result
		c = (c - (k+1) * pol[j-1]) % MOD; // from -x(k+1) P_{k}(x)
		c = (c + (j-1) * pol[j-1]) % MOD; // from x^2 P'_{k}(x)
		if (j <= k) c = (c - j * pol[j]) % MOD; // from -x P'_{k}(x)
		if (c < 0) c += MOD;
		res[j] = c;
	}
	return res;
}

// Calculates \sum_{i = 0}^{n-1} r^{i} i^{k} in O(k^2) time and O(k) memory
// r cannot be 1!
ll geoPowSum(ll n, ll r, int k) {
	// 1. Calculate P_{j} and g(j) for 0 <= j <= k
	ll div = modPow((MOD+r-1) % MOD, MOD - 2);
	vector<ll> pol = {MOD - 1};
	vector<ll> g(k+1); // g[j] = \sum_{i = 0}^{\infty} r^{i} i^{j}
	for (int j = 0; j <= k; ++j) {
		g[j] = eval(pol, r) * modPow(div, j+1) % MOD;
		pol = nxt(pol);
	}
	// 2. Calculate factorials and binomials
	vector<ll> fact(k+1, 1);
	for (int i = 1; i <= k; ++i) fact[i] = i * fact[i-1] % MOD;

	vector<ll> binom(k+1);
	for (int i = 0; i <= k; ++i) {
		binom[i] = fact[k] * modPow(fact[k-i] * fact[i] % MOD, MOD-2) % MOD;
	}
	// 3. Calculate answer
	// res  = \sum_{i = 0}^{n-1} r^{i} i^{k}
	//	= g[k] - \sum_{i = 0}^{k} r^{n} n^{k-i} (k choose i) g[i]
	ll res = g[k];
	ll sub = (MOD - modPow(r, n)) % MOD;
	for (int i = 0; i <= k; ++i) {
		ll mult1 = sub * modPow(n, k-i) % MOD;
		ll mult2 = binom[i] * g[i] % MOD;
		res = (res + mult1 * mult2) % MOD;
	}
	return res;
}
