#include <iostream>
using namespace std;
using ll = long long;
constexpr int MOD = 1e9 + 7;

template<class T>
T itPow(T a, ll d, T r = 1) {
	for (ll b = 1; b <= d; b <<= 1) {
		if (d & b) r = r*a;
		a = a*a;
	}
	return r;
}

// Assumes the modulo is a prime
template<int P>
struct ModNum {
	int v;

	ModNum(int val) : v(val % P) {}
	ModNum inv() const {
		return itPow(*this, P-2);
	}

	ModNum operator+(const ModNum& rhs) const {
		int res = v + rhs.v;
		return (res >= P ? res-P : res);
	}
	ModNum operator-(const ModNum& rhs) const {
		int res = v - rhs.v;
		return (res < 0 ? res+P : res);
	}
	ModNum operator*(const ModNum& rhs) const {
		return ((ll)v * rhs.v) % P;
	}
	ModNum operator/(const ModNum& rhs) const {
		return (*this) * rhs.inv();
	}
	operator int() const { return v; }
};

int main() {
	using mint = ModNum<MOD>;

	int a, b, c;
	cin >> a >> b >> c;
	mint x = a;
	mint y = b;
	cout << x+y << ' ' << x-y << ' ' << x*y << ' ' << x/y << '\n';
	cout << x << ' ' << x.inv() << ' ' << x*x.inv() << '\n';
	
	ll d;
	cin >> d;
	cout << itPow(x, d) << '\n';
}
