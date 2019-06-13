#include <iostream>
using namespace std;
using ll = long long;
constexpr int MOD = 1e9 + 7;

constexpr int bLog(ll v) { return v ? 63 - __builtin_clzll(v) : 0; }
template<class T>
T itPow(const T& a, ll d, T one = 1) {
        T r = one;
        for (ll b = 1ll << bLog(d); b > 0; b /= 2) {
                if (d > b) r = r*r;
                if (d & b) r = r*a;
        }
        return r;
}

// Assumes the modulo is a prime
template<int P=MOD>
struct ModNum {
	int v;

	ModNum(int val) : v(val % P) {}
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
	ModNum inv() const {
		return itPow(*this, P-2);
	}
	ModNum operator/(const ModNum& rhs) const {
		return (*this) * rhs.inv();
	}
	operator int() const { return v; }
};

int main() {
	int a, b, c;
	cin >> a >> b >> c;
	ModNum<> x = a;
	ModNum<> y = b;
	cout << x*y << " " << x+y << " " << x-y << '\n';
	ll d;
	cin >> d;
	ModNum<> z = c;
	cout << z.inv() << ' ' << itPow(z, d) << '\n';
}
