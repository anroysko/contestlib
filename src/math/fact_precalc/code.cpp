constexpr ll modPow(ll a, ll b, ll c) {
	return (b&1) ? (a * modPow(a, b-1, c) % c)
		: (b ? modPow(a*a % c, b/2, c) : 1);
}

struct FactPrecalc {
	private:
		const int P;
		vector<int> fact, inv_fact;
	public:
		FactPrecalc(int n, int p) : P(p), fact(n+1, 1), inv_fact(n+1, 1) {
			for (int i = 2; i <= n; ++i) fact[i] = (ll)fact[i-1] * i % P;
			inv_fact[n] = modPow(fact[n], P-2, P);
			for (int i = n-1; i >= 0; --i) inv_fact[i] = (ll)inv_fact[i+1] * (i+1) % P;
		}

		ll operator()(ll a, ll b) const {
			if (a < b) return 0;
			ll div = (ll)inv_fact[a-b] * inv_fact[b] % P;
			return div * fact[a] % P;
		}

		/*
		// binom(a, b) % P using Lucas's theorem, works even if a >= P.
		ll operator()(ll a, ll b) const {
			ll res = 1;
			while(b) {
				ll na = a / P, nb = b / P;
				a -= P*na; b -= P*nb;
				
				if (a < b) return 0;
				ll div = (ll)inv_fact[a-b] * inv_fact[b] % P;
				ll mult = div * fact[a] % P;
				res = res * mult % P;
				
				a = na, b = nb;
			}
			return res;
		}
		*/
		
		ll operator[](int i) const { return fact[i]; }
		ll inv(int i) const { return inv_fact[i]; }
};
