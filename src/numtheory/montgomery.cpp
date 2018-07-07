#include <iostream>
using namespace std;
typedef long long ll;

ll modPow(ll a, ll b, ll c) {
	if (b & 1) return (modPow(a, b-1, c) * a) % c;
	if (b == 0) return 1;
	return modPow((a*a)%c, b >> 1, c);
}

// Enables 3 times faster modular multiplication, compared to using non-const modulo.
struct Montgomery {
	int m; // modulo
	int r; // the power of two
	int j; // r = 1<<j
	int ir; // Inverse of the power of two
	int rr; // r^2 mod m
	int mc; // r * ir - m * mc = 1

	inline int init(int p) {
		m = p;
		r = 1;
		j = 0;
		while(r <= m) {
			r <<= 1;
			++j;
		}
		ir = modPow(r, m-2, m);
		rr = ((ll)r*r) % m;
		mc = (ll)r*ir / m;
	}
	inline int mult(int a, int b) {
		ll t = (ll)a * b;
		ll tmc = ((t & (r-1)) * mc) & (r-1);
		ll u = (t + (m * tmc)) >> j;
		return (u >= m ? u-m : u);
	}
	inline int transform(int v) {
		return mult(v, rr);
	}
	inline int reverse(int v) {
		return mult(v, 1);
	}
};

int main() {
	const int MOD = 1e9 + 7;
	int val, mod, mult, pow;
	int loop_to_run;
	std::cin >> mult >> mod >> pow >> loop_to_run;
	val = 1;

	if (loop_to_run == 0) {
		Montgomery mont;
		mont.init(mod);
		val = mont.transform(val);
		mult = mont.transform(mult);
		for (int i = 0; i < pow; ++i) {
			val = mont.mult(val, mult);
		}
		val = mont.reverse(val);
	} else if (loop_to_run == 1) {
		for (int i = 0; i < pow; ++i) val = ((ll)val * mult) % mod;
	} else if (loop_to_run == 2) {
		for (int i = 0; i < pow; ++i) val = ((ll)val * mult) % MOD;
	}

	std::cout << val << '\n';
}
