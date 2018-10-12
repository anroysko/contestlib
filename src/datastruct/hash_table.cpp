#include <iostream>
#include <unordered_map>
#include <ext/pb_ds/assoc_container.hpp>
#include <chrono>
using namespace std;
using namespace __gnu_pbds;
typedef long long ll;
typedef unsigned long long ull;
const ull FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();

// https://codeforces.com/blog/entry/62393
struct CustomHash {
	static ull splitmix64(ull x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(ull x) const {
		return splitmix64(x + FIXED_RANDOM);
	}
};

int main() {
	// With input 1000000 100 1337 696969
	// std: 18.109
	// pbds: 3.482s
	unordered_map<ll, int, CustomHash> tab1;
	gp_hash_table<ll, int, CustomHash> tab2;
	
	ll n, k, a, b;
	cin >> n >> k >> a >> b;
	
	for (int i = 0; i < n; ++i) {
		// tab1[(i*a)^b] = i;
		tab2[(i*a)^b] = i;
	}
	for (int j = 0; j < k; ++j) {
		for (int i = 0; i < n; ++i) {
			// ++tab1[(i*a)^b];
			++tab2[(i*a)^b];
		}
	}
	ll res = 0;
	for (int i = 0; i < n; ++i) {
		// res += tab1[(i*a)^b];
		res += tab2[(i*a)^b];
	}
	cout << res << '\n';
}
