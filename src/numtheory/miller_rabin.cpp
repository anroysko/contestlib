#include <iostream>
using namespace std;
typedef __int128 lll;
typedef long long ll;

// Deterministic Miller-Rabin
// https://miller-rabin.appspot.com/
// 128-bit integers are required for testing 64-bit numbers.
lll modPow(lll w, lll u, lll val) {
	if (u & 1) return (w * modPow(w, u-1, val)) % val;
	if (u == 0) return 1;
	lll sub = modPow(w, u>>1, val);
	return (sub * sub) % val;
}

bool isWitness(lll w, lll even, lll odd, lll val) {
	w = modPow(w, odd, val);
	if (w == 1) return 0;
	for (int i = 0; i < even; ++i) {
		lll nw = (w * w) % val;
		if (nw == 1) return (w != val-1);
		w = nw;
	}
	return 1;
}

bool isPrime(ll val) {
	if (val <= 1 || ((val & 1) == 0 && val > 2)) return 0;
	ll even = 0;
	ll odd = val-1;
	while(!(odd & 1)) {
		++even;
		odd >>= 1;
	}
	ll b[7] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
	for (int i = 0; i < 7; ++i) {
		ll w = b[i] % val;
		if (w == 0) return 1;
		if (isWitness(w, even, odd, val)) return 0;
	}
	return 1;
}

int main() {
	for (int i = 1; i <= 100; ++i) cout << isPrime(i); cout << '\n';
}
