#include <iostream>
#define sce static constexpr
#define ce constexpr
using namespace std;
using ll = long long;
using ull = unsigned long long;
using uint = unsigned int;

ce ull modPow(ull a, ull b, ull c) {
	if (b & 1) return (modPow(a, b-1, c) * a) % c;
	if (b == 0) return 1;
	return modPow((a*a)%c, b >> 1, c);
}
ce ull modInv(ull a, ull p) {
	return modPow(a, p-2, p);
}

struct Montgomery {
	sce ull R = (ull)1 << 32;
	const uint P;
	const ull RR, MC;

	ce Montgomery(uint m) : P(m), RR((R%m)*R % m),
		MC(R * modInv(R % m, m) / m) {}

	// Inputs must be < 2P. Output will be < 2P
	ce uint mult(uint a, uint b) const {
		ull t = (ull)a * b;
		ull tmc = (uint)t * MC;
		return (t + (P * tmc)) >> 32; // Exact division!!
	}
	ce uint transform(uint v) const {
		return mult(v, RR);
	}
	ce uint reverse(uint v) const {
		return mult(v, 1);
	}
};

int main() {
	ce uint MOD = 1000000007;
	ce Montgomery mont(MOD);

	ce uint a = 1;
	ce uint b = 1;
	ce uint va = mont.transform(a);
	ce uint vb = mont.transform(b);
	ce uint vc = mont.mult(va, vb);
	ce uint c = mont.reverse(vc);
	cout << mont.P << ' ' << mont.RR << ' ' << mont.MC << '\n';
	cout << a << ' ' << b << ' ' << va << ' ' << vb << ' ' << vc << ' ' << c << '\n';
}
