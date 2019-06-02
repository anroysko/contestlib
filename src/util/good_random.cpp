#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
using ll = long long;

// rand-function that works properly on windows, and is faster than rand
// https://codeforces.com/blog/entry/61587
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template<class T>
T rand(T a, T b) {
	return uniform_int_distribution<T>(a, b)(rng);
}
template<class T>
T rand() {
	return uniform_int_distribution<T>()(rng);
}

int main() {
	int a, b, n;
	cin >> a >> b >> n;
	vector<int> perm(n);
	for (int i = 0; i < n; ++i) perm[i] = i;

	// Taking random number in range. Is faster, fails to b < a-1, and infers the type
	cout << rand(a, b) << '\n'; // Segfaults if b < a

	// This is a bit slower, but works like rand() usually does.
	cout << rand<int>() % (b-a+1) + a << '\n';

	// random_shuffle uses rand(), and therefore doesn't work well on windows
	shuffle(perm.begin(), perm.end(), rng);
	for (auto v : perm) cout << v << ' '; cout << '\n';
}
