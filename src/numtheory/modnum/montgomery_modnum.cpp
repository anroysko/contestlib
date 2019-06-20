#include <iostream>
#define sce static constexpr
#define ce constexpr
using namespace std;
using ll = long long;
using ull = unsigned long long;
using uint = unsigned int;

ce ull modPow(ull a, ull b, ull c) {
	ull res = 1;
	for (ull bit = 1; bit <= b; bit <<= 1) {
		if (b & bit) res = (res * a) % c;
		a = (a * a) % c;
	}
	return res;
}
template<class T>
T itPow(T a, ll d, T r = 1) {
        for (ll b = 1; b <= d; b <<= 1) {
                if (d & b) r = r*a;
                a = a*a;
        }
        return r;
}


template<uint P>
class ModNum {
	private:
		sce ull R = (ull)1 << 32;
		sce ull RM = R % P;
		sce ull IR = modPow(RM, P-2, P);
		sce uint RR = (RM*RM) % P;
		sce uint MC = (R*IR) / P;

		sce uint mult(uint a, uint b) {
			ull t = (ull)a * b;
			ull tmc = (uint)t * MC;
			return (t + (P * tmc)) >> 32; // Exact division!!
		}
		sce uint modOnce(uint v, uint m) {
			return (v >= m ? v - m : v);
		}
		sce ModNum build(uint v) {
			ModNum tmp;
			tmp.val = v;
			return tmp;
		}

		uint val; // The transformed value
	public:
		ce ModNum() : val(0) {}
		ce ModNum(uint v) : val(mult(v % P, RR)) {}

		// pow(-1) for inverse. Must have b \in (-P, INF)
		ce ModNum pow(ll b) const {
			return itPow(*this, (b < 0 ? b+P-1 : b));
		}
		ce ModNum operator+(ModNum rhs) const {
			return build(modOnce(val + rhs.val, 2*P));
		}
		ce ModNum operator-(ModNum rhs) const {
			return build(modOnce(2*P + val - rhs.val, 2*P));
		}
		ce ModNum operator*(ModNum rhs) const {
			return build(mult(val, rhs.val));
		}
		ce ModNum operator/(ModNum rhs) const {
			return (*this) * rhs.pow(-1);
		}
		ce operator int() const {
			return modOnce(mult(val, 1), P);
		}
};

// NOTE: requires c++14 or newer for the constant expression stuff.
int main() {
	ce uint MOD = 1e9 + 7;
	using mint = ModNum<MOD>;

	ce mint a = 7;
	ce mint b = 15;
	cout << a+b << ' ' << a-b << ' ' << a*b << ' ' << a/b << '\n';
	cout << a << ' ' << a.pow(-1) << ' ' << a*a.pow(-1) << '\n';

	int xv, yv;
	cin >> xv >> yv;
	mint x = xv;
	mint y = yv;
	cout << x+y << ' ' << x-y << ' ' << x*y << ' ' << x/y << '\n';
	cout << x << ' ' << x.pow(-1) << ' ' << x*x.pow(-1) << '\n';
}
