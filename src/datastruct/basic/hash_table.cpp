#include <chrono>
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
using ull = unsigned long long;
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

// unordered_map<ll, int, CustomHash> tab1;
// gp_hash_table<ll, int, CustomHash> tab2;
