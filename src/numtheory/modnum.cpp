#include <iostream>
using namespace std;
using ll = long long;
constexpr int MOD = 1e9 + 7;

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
	ModNum pow(ll d) const {
		ModNum r(1);
		for (ll b = 1ll<<62; b > 0; b /= 2) {
			if (d > b) r = r*r;
			if (d & b) r = r*(*this);
		}
		return r;
	}
	ModNum inv() const {
		return this->pow(P-2);
	}
	ModNum operator/(const ModNum& rhs) const {
		return (*this) * rhs.inv();
	}
	operator int() const { return v; }
};

int main() {
	int a, b;
	cin >> a >> b;
	ModNum<> x = a;
	ModNum<> y = b;
	cout << x*y << " " << x+y << " " << x-y << '\n';
	int c, k;
	cin >> c >> k;
	ModNum<> z = c;
	cout << z.inv() << ' ' << z.pow(k) << '\n';
}
