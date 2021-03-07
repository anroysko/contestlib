#include <iostream>
#include <array>
using namespace std;
using ll = long long;

constexpr ll modPow(ll a, ll b, ll c) {
	return (b & 1) ? (a * modPow(a, b^1, c) % c) : (b == 0 ? 1 : modPow(a*a % c, b >> 1, c));
}

// Garner's algorithm for chinese remainder theorem in O(|P|^2)
// https://cp-algorithms.com/algebra/chinese-remainder-theorem.html
template<int MOD, int... P>
constexpr int garner(array<int, sizeof...(P)> r) {
	constexpr int n = sizeof...(P);
	constexpr array<int, n> p = {P...};

	constexpr auto inv = [p]() {
		array<array<ll, n>, n> res{};
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < i; ++j) {
				res[i][j] = modPow(p[j], p[i]-2, p[i]);
			}
		}
		return res;
	}();
	constexpr auto mult = [p]() {
		array<ll, n> res{};
		for (
	}

	ll res = 0, mult = 1;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < i; ++j) {
            r[i] = ((ll)r[i] - r[j]) * inv[i][j] % p[i];
		}
		res = (res + mult * r[i]) % MOD;
		mult = mult * p[i] % MOD;
	}
	return (res < 0 ? res + MOD : res);
}

int main() {
	const int P0 = (int)1e9 + 7;
	const int P1 = (int)1e9 + 9;
	const int P2 = (int)1e9 + 21;
	const int P3 = (int)1e9 + 33;
	const int MOD = 31;

	array<int, 4> r;
	for (int& v : r) cin >> v;

	int res = garner<MOD, P0, P1, P2, P3>(r);
	cout << res << '\n';
}

