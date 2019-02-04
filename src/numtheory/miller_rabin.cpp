#include <iostream>
#include <vector>
using namespace std;
typedef __int128 lll;
typedef long long ll;

// Deterministic Miller-Rabin
// https://miller-rabin.appspot.com/
// 128-bit integers are required for testing 64-bit numbers.
lll modPow(lll a, lll b, lll mod) {
	if (b & 1) return (a * modPow(a, b-1, mod)) % mod;
	if (b == 0) return 1;
	return modPow((a * a) % mod, b / 2, mod);
}
bool isWitness(lll w, lll ctz, lll odd, lll x) {
	w = modPow(w, odd, x);
	if (w == 1) return 0;
	while(ctz && (w != x - 1)) {
		w = (w * w) % x;
		--ctz;
	}
	return ctz >= 1;
}
bool isPrime(ll x) {
	if (x <= 1 || (x % 2 == 0 && x > 2)) return 0;
	ll ctz = __builtin_ctz(x-1);
	ll odd = (x-1) >> ctz;
	for (auto b : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
		ll w = b % x;
		if (w == 0) return 1;
		if (isWitness(w, ctz, odd, x)) return 0;
	}
	return 1;
}

int main() {
	for (int i = 1; i <= 100; ++i) cout << isPrime(i); cout << '\n';
}
