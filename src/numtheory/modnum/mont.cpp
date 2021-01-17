using ull = unsigned long long;
using uint = unsigned int;

// Requires C++14 or newer!
// Class for faster modular multiplication by non-constant prime modulo P < 2^30.
class Montgomery {
	private:
		static constexpr ull R = (ull)1 << 32;
		const uint P, RR, MC; // Modulo, 2^64 mod P, x for which xP = -1 mod 2^32

		static ull modPow(ull a, ull b, ull c) {
			ull r = 1;
			for (; b; b /= 2) {
				if (b & 1) r = (r*a) % c;
				a = (a*a) % c;
			}
			return r;
		}
	public:
		Montgomery(uint m) : P(m), RR((R%m)*R % m), MC((R * modPow(R % m, m-2, m) - 1) / m) {}

		uint mult(uint a, uint b) const {
			ull t = (ull)a * b; // < 4P
			ull k = (uint)t * MC; // < 2^32
			return (t + k * P) >> 32; // < 2P, Exact division!
		}
		uint add(uint a, uint b) const {
			return (a+b < 2*P ? a+b : a+b - 2*P);
		}
		uint sub(uint a, uint b) const {
			return (a < b) ? (2*P - b + a) : (a - b);
		}
		uint pow(uint v, ll b) const {
			uint r = transform(1);
			for (b += P-1; b; b >>= 1) {
				if (b & 1) r = mult(r, v);
				v = mult(v, v);
			}
			return r;
		}
		uint transform(uint v) const {
			return mult(v, RR);
		}
		uint reverse(uint v) const {
			uint res = mult(v, 1);
			return (res < P) ? res : (res - P);
		}
};

class ModNum {
	private:
		static Montgomery m = Montgomery((uint)1e9 + 7);
		uint v;

		ModNum(uint x, bool raw) : v(x) {}
	public:
		static void setMod(ll mod) { m = Montgomery(mod); }
		ModNum(uint x) : v(m.transform(x)) {}
		ModNum(const ModNum& rhs) : v(rhs.v) {}
		operator int() const { return m.reverse(v); }

		ModNum operator+(const ModNum& rhs) const { return ModNum(m.add(v, rhs.v), 1); }
		ModNum operator-(const ModNum& rhs) const { return ModNum(m.sub(v, rhs.v), 1); }
		ModNum operator*(const ModNum& rhs) const { return ModNum(m.mult(v, rhs.v), 1); }
		ModNum operator/(const ModNum& rhs) const { return ModNum(m.mult(v, m.pow(rhs.v, P-2)), 1); }
		ModNum pow(ll b) const { return ModNum(m.pow(v, b), 1); }
};
