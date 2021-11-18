#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
using uint = unsigned int;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

#include "code.cpp"

const uint mx = -1;
const uint mx2 = 1e3;
const uint zero = 0;
const uint one = 1;

uint randNum(bool can_be_zero) {
	int t = rand(0, 2 + can_be_zero);
	if (t == 0) {
		return rand(one, mx);
	} else if (t == 1) {
		return rand(one, mx2);
	} else if (t == 2) {
		return one << rand(0, 31);
	} else {
		return 0;
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	for (int it = 0;; ++it) {
		ull d = randNum(0);
		FastMod mod(d);
		cout << it << ' ' << d << endl;

		for (int it2 = 0; it2 < 1e6; ++it2) {
			ull x = randNum(1);
			ull res = mod(x);
			assert(res == x % d);
			assert(mod.zeroRem(x) == ((x % d) == 0));
		}
	}
}
