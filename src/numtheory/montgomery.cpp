#include <iostream>
#include <assert.h>
using namespace std;
typedef long long ll;
typedef unsigned long long ull;

ull modPow(ull a, ull b, ull c) {
	if (b & 1) return (modPow(a, b-1, c) * a) % c;
	if (b == 0) return 1;
	return modPow((a*a)%c, b >> 1, c);
}

// Enables 3 times faster modular multiplication, compared to using non-const modulo.
// Initialized with the modulo. Then after that, you can transform numbers into montgomery
// form, transform them back, and multiply them. Multiplication in montgomery form is
// much faster than normal multiplication, because no modulo operations are required.

// To achieve even more speed, we always use r = 2^32.
// Preconditions: p is odd (doesn't have to be prime)
struct Montgomery {
	unsigned m; // modulo
	unsigned rr; // r^2 mod m
	unsigned mc; // r * ir - m * mc = 1

	void init(int p) {
		m = p;
		ull r = (ull)1 << 32;
		ull rm = r % m;
		ull ir = modPow(rm, m-2, m); // inverse of r mod m
		rr = (rm*rm) % m;
		mc = (r*ir) / m; // since r*ir = 1 (mod m), no need to -1.
	}

	// Say that r > 4m, and a, b < 2m. Now, the output is < 2m.
	// Due to this we need to use unsigned integers (in order for 10^9 + 7 to work)
	// Thankfully 2m fits into integer normally. We just need to use unsigneds internally.
	// If we use 2^32, calculating modulo 2^32 is even faster! That's why it is always used.
	int mult(int a, int b) {
		ull t = (ull)a * b;
		ull tmc = (unsigned)t * mc;
		return (t + (m * tmc)) >> 32; // Exact division!!
	}
	int transform(int v) {
		return mult(v, rr);
	}
	int reverse(int v) {
		return mult(v, 1);
	}
};

int main() {
	const int MOD = 1e9 + 7;
	int val, mod, mult, pow, loop_to_run;
	std::cout << "mod mult pow loop\n";
	std::cin >> mod >> mult >> pow >> loop_to_run;
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
		if (val >= mod) val -= mod;
	} else if (loop_to_run == 1) {
		for (int i = 0; i < pow; ++i) val = ((ll)val * mult) % mod;
	} else if (loop_to_run == 2) {
		for (int i = 0; i < pow; ++i) val = ((ll)val * mult) % MOD;
	}

	std::cout << val << '\n';
}
