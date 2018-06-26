#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;
const int MOD = 1e9 + 7;

// Calculates a^b % MOD
// Time Complexity: O(log b)
ll modPow(ll a, ll b) {
	if (b & 1) return (a * modPow(a, b-1)) % MOD;
	if (b == 0) return 1 % MOD;
	return modPow((a * a) % MOD, b >> 1);
}

// Calculates modular inverse of v.
// (v * modInv(v)) % MOD = 1, modInv(0) = 0
// Time Complexity: O(log MOD)
ll modInv(ll v) {
	return modPow(v, MOD - 2);
}

// Fast hadamard transform (FHT)
// Calculates the fast hadamard transform of the given vector * sqrt(N) % MOD
// Transform: [1  1] Inverse: [1/2  1/2]
//            [1 -1]          [1/2 -1/2]
// Preconditions: n (size of vec) must be a power of two
// Time complexity: O(n log n)
void xorFHT(vector<int>& vec, bool inv) {
	int n = vec.size();
	for (int len = 1; len < n; len *= 2) {
		for (int b = 0; b < n; b += 2 * len) {
			for (int d = 0; d < len; ++d) {
				int left = vec[b + d];
				int right = vec[b + d + len];
				vec[b + d] = (left + right) % MOD;
				vec[b + d + len] = (left - right + MOD) % MOD;
			}
		}
	}
	if (inv) {
		for (int i = 0; i < n; ++i) vec[i] = (vec[i] * modInv(n)) % MOD;
	}
}

// And-version of Fast hadamard transform (FHT) % MOD
// Transform: [0 1] Inverse: [-1 1]
//            [1 1]          [ 1 0]
// Preconditions: n (size of vec) must be a power of two
// Time complexity: O(n log n)
void andFHT(vector<int>& vec, bool inv) {
	int n = vec.size();
	for (int len = 1; len < n; len *= 2) {
		for (int b = 0; b < n; b += 2 * len) {
			for (int d = 0; d < len; ++d) {
				int left = vec[b + d];
				int right = vec[b + d + len];
				if (inv) {
					vec[b + d] = (right - left + MOD) % MOD;
					vec[b + d + len] = left;
				} else {
					vec[b + d] = right;
					vec[b + d + len] = (left + right) % MOD;
				}
			}
		}
	}
}

// Or-version of Fast hadamard transform (FHT) % MOD
// Transform: [1 1] Inverse: [1 -1]
//            [0 1]          [0  1]
// Preconditions: n (size of vec) must be a power of two
// Time complexity: O(n log n)
void orFHT(vector<int>& vec, bool inv) {
	int n = vec.size();
	for (int len = 1; len < n; len *= 2) {
		for (int b = 0; b < n; b += 2 * len) {
			for (int d = 0; d < len; ++d) {
				int left = vec[b + d];
				int right = vec[b + d + len];
				if (inv) {
					vec[b + d] = left;
					vec[b + d + len] = (right - left + MOD) % MOD;
				} else {
					vec[b + d] = left;
					vec[b + d + len] = (left + right) % MOD;
				}
			}
		}
	}
}

// Xor-combines vectors A and B. Result is C % MOD, such that
// C_{i} = \sum_{ai, bi : ai ^ bi = i} A_{ai} * B_{bi}
// Preconditions: n (size of A, B) must be a power of two
// Time complexity: O(n log n)
vector<int> xorCombine(vector<int> a, vector<int> b) {
	xorFHT(a, false); // Transform a
	xorFHT(b, false); // Transform b
	for (int i = 0; i < a.size(); ++i) a[i] = ((ll)a[i] * b[i]) % MOD; // Combine them
	xorFHT(a, true); // Inverse the transform
	return a;
}
// And-combines vectors A and B. Result is C % MOD, such that
// C_{i} = \sum_{ai, bi : ai & bi = i} A_{ai} * B_{bi}
// Preconditions: n (size of A, B) must be a power of two
// Time complexity: O(n log n)
vector<int> andCombine(vector<int> a, vector<int> b) {
	andFHT(a, false); // Transform a
	andFHT(b, false); // Transform b
	for (int i = 0; i < a.size(); ++i) a[i] = ((ll)a[i] * b[i]) % MOD; // Combine them
	andFHT(a, true); // Inverse the transform
	return a;
}
// Or-combines vectors A and B. Result is C % MOD, such that
// C_{i} = \sum_{ai, bi : ai | bi = i} A_{ai} * B_{bi}
// Preconditions: n (size of A, B) must be a power of two
// Time complexity: O(n log n)
vector<int> orCombine(vector<int> a, vector<int> b) {
	orFHT(a, false); // Transform a
	orFHT(b, false); // Transform b
	for (int i = 0; i < a.size(); ++i) a[i] = ((ll)a[i] * b[i]) % MOD; // Combine them
	orFHT(a, true); // Inverse the transform
	return a;
}


int main() {
	vector<int> a = {12, 34, 56, 78};
	vector<int> b = {21, 43, 65, 87};
	vector<int> c;

	c = xorCombine(a, b); // Should be: 12140 11172 8268 7300 
	for (auto it : c) cout << it << ' '; cout << '\n';
	c = andCombine(a, b); // Should be: 10738 7774 13582 6786 
	for (auto it : c) cout << it << ' '; cout << '\n';
	c = orCombine(a, b); // Should be: 252 2692 5596 30340
	for (auto it : c) cout << it << ' '; cout << '\n';
}
