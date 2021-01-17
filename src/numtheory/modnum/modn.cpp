template<ll P>
struct ModNum {
	ll v;
	ModNum(ll val) : v(val) {}
	operator ll() const { return v; }

	ModNum operator+(const ModNum& rhs) { ll res = v + rhs.v; return res >= MOD ? res - MOD : res; }
	ModNum operator-(const ModNum& rhs) { ll res = v - rhs.v; return res >= 0 ? res : res + MOD; }
	ModNum operator*(const ModNum& rhs) { return v * rhs.v % MOD; }
	ModNum operator/(const ModNum& rhs) { return v * rhs.pow(MOD - 2) % MOD; }
	ModNum& operator+=(const ModNum& rhs) { v += rhs.v; if (v >= MOD) v -= MOD; return *this; }
	ModNum& operator-=(const ModNum& rhs) { v -= rhs.v; if (v < 0) v += MOD; return *this; }
	ModNum& operator*=(const ModNum& rhs) { v = v * rhs.v % MOD; return *this; }
	ModNum& operator/=(const ModNum& rhs) { v = v * rhs.pow(MOD - 2) % MOD; return *this; }
	
	ModNum pow(ll b) const {
		ModNum res(1), cur(v);
		for (ll j = 1; j <= b; j <<= 1) {
			if (b & j) res *= cur;
			cur *= cur;
		}
		return res;
	}
};
