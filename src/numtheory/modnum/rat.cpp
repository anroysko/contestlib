// ModNum for O(1) division. Other operations are slower, and casting is O(log H)
template<ll P>
struct ModNum {
	private:
		ll a, b; // a/b

		ll pw(ll x, ll y, ll res = 1) const {
			for (ll j = 1; j <= y; j <<= 1) {
				if (y & j) res = (res * x) % P;
				x = (x * x) % MOD;
			}
			return res;
		}
		ModNum(ll num, ll div) : a(num), b(div) {}
	public:
		ModNum(ll val) : a(val), b(1) {}
		operator ll() const { return a * pw(b, P - 2) % P;  }
		operator int() const { return a * pw(b, P - 2) % P; }

		ModNum operator+(const ModNum& rhs) { return ModNum((a * rhs.b + b * rhs.a) % P, b * rhs.b % P); }
		ModNum operator-(const ModNum& rhs) {
			ll num = (a * rhs.b - b * rhs.a) % P;
			if (num < P) num += P;
			return ModNum(num, b * rhs.b % P);
		}
		ModNum operator*(const ModNum& rhs) { return ModNum(a * rhs.a % P, b * rhs.b % P); }
		ModNum operator/(const ModNum& rhs) { return ModNum(a * rhs.b % P, b * rhs.a % P); }
	
};
using num = ModNum<MOD>;
