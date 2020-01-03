#include <iostream>
#include <vector>
using namespace std;
using ll = long long;
const int MOD = (int)1e9 + 7;

#include "code.cpp"

int main() {
	ll a, b, m;
	bool t;
	cin >> a >> b >> m >> t;

	if (t == 0) {
		ll res = 1;
		if (m == -1) {
			for (ll i = 0; i < b; ++i) res = (res * a) % MOD;
		} else {
			for (ll i = 0; i < b; ++i) res = (res * a) % m;
		}
		cout << res << '\n';
	} else {
		if (m == -1) {
			auto res = ModNum<MOD>::init(1);
			auto mult = ModNum<MOD>::init(a);
			for (ll i = 0; i < b; ++i) res = res * mult;
			cout << res << '\n';
		} else {
			Montgomery mt(m);
			int res = mt.transform(1);
			int mult = mt.transform(a);
			for (ll i = 0; i < b; ++i) res = mt.mult(res, mult);
			cout << mt.reverse(res) << '\n';
		}
	}
}
